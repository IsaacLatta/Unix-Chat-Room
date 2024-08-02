#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <poll.h>
#include "client.h"
#include "../Shared/protocol.h"
#include "filetransferhandler.h"

typedef struct 
{
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addr_len;
    int client_count;
    Client clients[MAX_CLIENTS];
    struct pollfd fds[2];
    FileTransferHandler_t ft_handler;
} Server;

void init_server(Server* server, int port);
void free_server(Server* server);
void register_client(Server* server, Client* client);
void remove_client(Server* server, Client* cleint);
void add_client(Server* server, Client* client);
void relay_msg(Server* server, Client* sender, char* msg, int msg_len);
void relay_all(Server* server, char* msg, int msg_len);
void send_data(Server* server, Client* client, char* buffer, int buffer_size, Header_t* header);
void recv_data(Server* server, Client* client, char* buffer, int buffer_size);
void handle_header(Server* server, Client* client, Header_t* header);

#endif