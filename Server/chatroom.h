#ifndef CHATROOM_H
#define CHATROOM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include "server.h"

typedef struct
{
    Server server;
    int isRunning;
    struct pollfd fds[2];
} ChatRoom;

void init_chatroom(ChatRoom* chatroom, int port);
void run_chatroom(ChatRoom* chatroom);
void close_chatroom(ChatRoom* chatroom);
void process_fds(ChatRoom* chatroom);
void handle_fd(ChatRoom* chatroom, int fd);
void handle_kb(ChatRoom* chatroom);
void handle_msg(ChatRoom* chatroom);
void signal_shutdown(ChatRoom* chatroom);
#endif