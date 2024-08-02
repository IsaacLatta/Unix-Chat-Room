#ifndef FILETRANSFERHANDLER_H
#define FILETRANSFERHANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include "../Shared/protocol.h"

typedef struct
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    int type;
    int sock;
} ft_request_t;

typedef struct 
{
    int tcp_sock;
    int is_running;
    struct sockaddr_in addr;
    socklen_t addr_len;
    pthread_t thread_id;
    fd_set sockets[MAX_CLIENTS];
    // threadpool
} FileTransferHandler_t;

void init_handler(FileTransferHandler_t* ft_handler, int port, int max_clients);
void run_handler(FileTransferHandler_t* ft_handler);
void stop_handler(FileTransferHandler_t* ft_handler);
void* get_connections(void* arg);
void* poll_sockets(void* ft_handler);
void add_socket(FileTransferHandler_t* ft_handler);
void* handle_transfer(void* ft_handler, void* ft_request_t);

#endif