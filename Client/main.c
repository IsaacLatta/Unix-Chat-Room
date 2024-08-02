#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "chatroom_client.h"

#define PORT 5555
#define SERVER_IP "192.168.1.69"

int main()
{
    ChatRoom_Client client;
    init_client(&client, SERVER_IP, PORT);
    run_chatroom_client(&client);
}
