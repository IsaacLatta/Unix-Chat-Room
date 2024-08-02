#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>

typedef struct
{
    struct sockaddr_in addr;
    socklen_t addr_len;
    int initialized;
} Client;

void init_client(Client* client);
int is_same_client(const Client* c1, const Client* c2);

#endif