#include "chatroom.h"

void init_chatroom(ChatRoom* chatroom, int port)
{
    chatroom->isRunning = 1;
    init_server(&chatroom->server, port);

    chatroom->fds[0].fd = chatroom->server.sockfd;
    chatroom->fds[0].events = POLLIN;
    chatroom->fds[1].fd = STDIN_FILENO;
    chatroom->fds[1].events = POLLIN;
}

void process_fds(ChatRoom* chatroom)
{
    int res = poll(chatroom->fds, 2, 50);
    if(res == -1)
    {
        exit_on_error("[-] poll() failed");
    }

    for(int i = 0; i < 2; i++)
    {
        if(chatroom->fds[i].revents & POLLIN)
        {
            handle_fd(chatroom, chatroom->fds[i].fd);
        }
    }
}

void handle_kb(ChatRoom* chatroom)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    read(STDIN_FILENO, buffer, BUFFER_SIZE);
    if(strcmp(buffer, "/exit\n") == 0)
    {
        signal_shutdown(chatroom);
    }
    relay_all(&chatroom->server, buffer, BUFFER_SIZE);
}

void handle_fd(ChatRoom* chatroom, int fd)
{
    if(fd == chatroom->fds[0].fd)
    {
        handle_msg(chatroom);
        return;
    }
    else if(fd == chatroom->fds[1].fd)
    {
        handle_kb(chatroom);
        return;
    }
}

void handle_msg(ChatRoom* chatroom)
{
    Client client;
    init_client(&client);
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    recv_data(&chatroom->server, &client, buffer, BUFFER_SIZE);
    register_client(&chatroom->server, &client);

    buffer[strcspn(buffer, "\n")] = '\0';
    printf("[*] Message: %s\n", buffer);
    relay_msg(&chatroom->server, &client, buffer, BUFFER_SIZE);
}


void close_chatroom(ChatRoom* chatroom)
{
    free_server(&chatroom->server);
}

void run_chatroom(ChatRoom* chatroom)
{
    while(chatroom->isRunning)
    {
        process_fds(chatroom);
    }
    close_chatroom(chatroom);
}

void signal_shutdown(ChatRoom* chatroom)
{
    chatroom->isRunning = 0;
}

