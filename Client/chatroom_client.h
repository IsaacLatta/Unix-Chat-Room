#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "../Shared/protocol.h"

#define BUFFER_SIZE 1024
#define MAX_THREADS 10

typedef struct
{
    struct sockaddr_in server_addr;
    int sockfd;
    const char* SERVER_IP;
    int is_running;
    int port;
    struct pollfd fds[2];
    int fd_count;
} ChatRoom_Client;

void init_client(ChatRoom_Client* client, const char* server_ip, int port);
void run_chatroom_client(ChatRoom_Client* client);
void recv_data(ChatRoom_Client* client, char* buffer, int buffer_size);
void send_data(ChatRoom_Client* client, char* buffer, int buffer_size, Header_t* header);
void handle_header(ChatRoom_Client* client , Header_t* header);
void process_fds(ChatRoom_Client* client);
void handle_fd(ChatRoom_Client* client, int fd);
void handle_kb(ChatRoom_Client* client);
void handle_msg(ChatRoom_Client* client);

