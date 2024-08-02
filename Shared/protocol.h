#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "util.h"

#define TYPE_MSG 1 // Reqular Message
#define TYPE_FILE_REQUEST 2 // Request Available files
#define TYPE_AVAILABLE_FILES 3 // List of available files
#define TYPE_ERROR 4

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct
{
    uint8_t data_type;
    int data_size;
} Header_t;

void init_header(Header_t* header, uint8_t type, int data_size);
char* add_header(char* buffer, int buffer_size, Header_t* header);
Header_t* strip_header(char* buffer, int buffer_size);

typedef struct
{
    uint8_t type;
    uint32_t filename_len;
    uint64_t filesize;
} FileTransferHeader_t;

void init_ft_header(FileTransferHeader_t* ft_header, uint8_t type, 
                    uint32_t filename_len, char* filename, uint64_t filesize);

#endif