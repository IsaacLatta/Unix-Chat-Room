#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Designed with thread safety in mind

typedef struct Node
{
    void* data;
    struct Node* next;
} node_t;

node_t* queue_create_node(void* data);

typedef struct 
{
    node_t* front;
    node_t* back;
    pthread_mutex_t lock;
    pthread_cond_t notify;
} queue_t;

queue_t* queue_create();
void queue_push(queue_t* queue, void* data); 
void* queue_pop(queue_t* queue); // Will sleep until a task is ready
void queue_destroy(queue_t* queue);
#endif 