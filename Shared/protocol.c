#include "protocol.h"

void init_header(Header_t* header, uint8_t type, int size)
{
    header->data_type = type;
    header->data_size = htonl(size);
}

char* add_header(char* buffer, int buffer_size, Header_t* header)
{
    char* data_to_send = (char*)ec_malloc(sizeof(Header_t) + buffer_size);
    memset(data_to_send, 0, sizeof(Header_t) +  buffer_size);
    memcpy(data_to_send, header, sizeof(Header_t));
    memcpy(data_to_send + sizeof(Header_t), buffer, buffer_size);
    return data_to_send;
}

Header_t* strip_header(char* buffer, int buffer_size)
{
    Header_t* header = (Header_t*)ec_malloc(sizeof(header));
    memcpy(header, buffer, sizeof(Header_t)); 
    memmove(buffer, buffer + sizeof(Header_t), buffer_size - sizeof(Header_t));
    memset(buffer + buffer_size - sizeof(Header_t), 0, sizeof(header));
    return header;
}