#include <string.h>
#include "client.h"

void init_client(Client* client)
{
    memset(&client->addr, 0, sizeof(client->addr));
    client->addr_len = sizeof(client->addr);
    client->initialized = 1;
}

int is_same_client(const Client* c1, const Client* c2)
{
    return ((c1->addr.sin_addr.s_addr == c2->addr.sin_addr.s_addr) &&
            (c1->addr.sin_port == c2->addr.sin_port));
}