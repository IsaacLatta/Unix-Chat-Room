#include "chatroom_client.h"

void init_client(ChatRoom_Client* client, const char* server_ip, int port)
{
    client->port = port;
    client->is_running = 1;
    client->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client->sockfd < 0)
    {
        exit_on_error("[-] Failed to create DGRAM socket");
    }
    client->fd_count = 2;
    client->fds[0].fd = client->sockfd;
    client->fds[0].events = POLLIN;
    client->fds[1].fd = STDIN_FILENO;
    client->fds[1].events = POLLIN;

    memset(&client->server_addr, 0 , sizeof(client->server_addr));
    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(port);
    client->SERVER_IP = server_ip;

    if(inet_pton(AF_INET, client->SERVER_IP, &client->server_addr.sin_addr) <= 0)
    {
        exit_on_error("[-] Invalid/Unsupported address");
    }
}

void recv_data(ChatRoom_Client* client, char* buffer, int buffer_size)
{
    recvfrom(client->sockfd, buffer, buffer_size, 0, NULL, NULL);
    Header_t* header = strip_header(buffer, buffer_size);
    handle_header(client, header);
    free(header);
}

void send_data(ChatRoom_Client* client, char* buffer, int buffer_size, Header_t* header)
{
    char* buffer_to_send = add_header(buffer, buffer_size, header);
    sendto(client->sockfd, buffer_to_send, buffer_size+sizeof(Header_t), 0, 
    (struct sockaddr*)&client->server_addr, sizeof(client->server_addr));
    free(buffer_to_send);
}

void run_chatroom_client(ChatRoom_Client* client)
{
    while(client->is_running)
    {
        process_fds(client);
    }
    close(client->sockfd);
}

void process_fds(ChatRoom_Client* client)
{
    int res = poll(client->fds, client->fd_count, 50); // 50ms timeout
    if(res == -1)
    {
        exit_on_error("[-] poll() failed");
    }

    for (int i = 0; i < client->fd_count; i++)
    {
        if(client->fds[i].revents & POLLIN)
        {
            handle_fd(client, client->fds[i].fd);
        }
    }
}

void handle_fd(ChatRoom_Client* client, int fd)
{
    if(fd == client->fds[0].fd)
    {
        handle_msg(client);
    }
    else if(fd == client->fds[1].fd)
    {
        handle_kb(client);
    }
    else
    {
        printf("[-] fd not found");
    }
}

void handle_msg(ChatRoom_Client* client)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    recv_data(client, buffer, BUFFER_SIZE);
    if(strcmp(buffer, "/exit\n") == 0)
    {
        client->is_running = 0;
    }
    //buffer[strcspn(buffer, "\n")] == '\0';
    printf("[*] Message: %s\n", buffer);
}

void handle_kb(ChatRoom_Client* client)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    read(STDIN_FILENO, buffer, BUFFER_SIZE);
    if(strcmp(buffer, "/exit\n") == 0)
    {
        client->is_running = 0;
    }
    if(strcmp(buffer, "/files\n") == 0)
    {
        Header_t header;
        init_header(&header, TYPE_FILE_REQUEST, BUFFER_SIZE);
        send_data(client, buffer, BUFFER_SIZE, &header);
        return;
    }
    if(strcmp(buffer, "/upload\n") == 0)
    {
        // launch seperate thread to get a tcp connection to server, and upload the file
        // perhaps launch an ft handler immediatly upon starting this program,
        // then call upload(ft_handler, file_name);
        return;
    }
    if(strcmp(buffer, "/download\n") == 0)
    {
        // download(ft_handler, file_name);
    }

    Header_t header;
    init_header(&header, TYPE_MSG, BUFFER_SIZE);
    send_data(client, buffer, BUFFER_SIZE, &header);
}

void handle_header(ChatRoom_Client* client, Header_t* header)
{
    printf("[*] Header Recv: %d\n", header->data_type);
}