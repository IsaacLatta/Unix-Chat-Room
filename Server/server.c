
#include "server.h"

void init_server(Server* server, int port)
{
    server->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server->sockfd < 0)
    {
        exit_on_error("[-] socket() failed");
    }
    server->fds[0].fd = server->sockfd;
    server->fds[0].events = POLLIN;
    server->fds[1].fd = STDIN_FILENO;
    server->fds[1].events = POLLIN;

    memset(&server->addr, 0, sizeof(server->addr));
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port);
    server->addr.sin_addr.s_addr = INADDR_ANY;
    server->addr_len = sizeof(server->addr);

    int res = bind(server->sockfd, (const struct sockaddr*)&server->addr, sizeof(server->addr));
    if (res < 0)
    {
        exit_on_error("[-] bind() failed");
    }

    server->client_count = 0;
    init_handler(&server->ft_handler, port, MAX_CLIENTS);
    run_handler(&server->ft_handler);
}

void handle_keyboard(Server* server)
{
    int res = poll(server->fds, 2, 50);
    if(res == -1)
    {
        exit_on_error("poll() failed");
    }
}

void free_server(Server* server)
{
    stop_handler(&server->ft_handler);
    if(pthread_join(server->ft_handler.thread_id, NULL) != 0)
    {
        exit_on_error("[-] pthread_join() failed on ft_handler");
    }
    printf("[+] Handler closed");
    close(server->sockfd);
}

void add_client(Server* server, Client* client)
{
    server->clients[server->client_count] = *client;
    server->client_count++;
}

void remove_client(Server* server, Client* client)
{
    for(int i = 0; i < server->client_count; i++)
    {
        if(is_same_client(client, &server->clients[i]))
        {
            for(int j = i; j < server->client_count -1; j++)
            {
                server->clients[j] = server->clients[j+1];
            }
            server->client_count--;
        }
    }
}

int client_exists(Server* server, Client* client)
{
    for(int i = 0; i < server->client_count; i++)
    {
        if(is_same_client(client, &server->clients[i]))
        {
            return 1;
        }
    }
    return 0;
}

void register_client(Server* server, Client* client)
{   
    if(server->client_count >= MAX_CLIENTS || client_exists(server, client))
    {
        return;
    }
    
   add_client(server, client);
}

void send_data(Server* server, Client* client, char* buffer, int buffer_size, Header_t* header)
{   
    char* buffer_to_send = add_header((char*)buffer, buffer_size, header);
    int bytes_sent = sendto(server->sockfd, buffer_to_send, sizeof(Header_t)+buffer_size, 0, 
                    (const struct sockaddr*)&client->addr, client->addr_len);
    if(bytes_sent < 0)
    {
        perror("sendto() failed");    
    }
    free(buffer_to_send);
}

void handle_header(Server* server, Client* client, Header_t* header)
{
    if(header->data_type == TYPE_FILE_REQUEST)
    {
        printf("[*] Header: TYPE_FILE_REQUEST\n");
        char* buffer = get_files("./files");
        Header_t header;
        init_header(&header, TYPE_AVAILABLE_FILES, strlen(buffer));
        send_data(server, client, buffer, strlen(buffer), &header);
        free(buffer);
    }
    else
    {
        printf("[*] Header: TYPE_MSG\n");
    }
}

void relay_all(Server* server, char* msg, int msg_len)
{
    printf("[*] Relay all was called\n");
    Header_t header;
    init_header(&header, TYPE_MSG, msg_len);
    for(int i = 0; i < server->client_count; i++)
    {
        send_data(server, &server->clients[i], msg, msg_len, &header);
    }
}

void relay_msg(Server* server, Client* sender, char* msg, int msg_len)
{
    Header_t header;
    init_header(&header, TYPE_MSG, msg_len);
    for(int i = 0; i < server->client_count; i++)
    {
        if(!is_same_client(sender, &server->clients[i]))
        {
            send_data(server, &server->clients[i], msg, msg_len, &header);
        }
    }
}

void recv_data(Server* server, Client* client, char* buffer, int buffer_size)
{
    int bytes = recvfrom(server->sockfd, buffer, buffer_size, 0, 
    (struct sockaddr*)&client->addr, &client->addr_len);
    printf("[*] Bytes recv: %d\n", bytes);

    Header_t* header = strip_header(buffer, buffer_size);
    handle_header(server, client, header);
    free(header);
}




