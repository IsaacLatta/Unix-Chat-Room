#include "queue.h"

node_t* queue_create_node(void* data)
{
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if(!node)
    {
        return NULL;
    } 
    node->data = data;
    node->next = NULL;
    return node;
}

queue_t* queue_create()
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    if(!queue)
    {
        return NULL;
    }
    queue->front = NULL;
    queue->back = NULL;
    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->notify, NULL);
    //printf("Queue state on creation: front=%p, back=%p\n", queue->front, queue->back);
    return queue;
}

void queue_push(queue_t* queue, void* data)
{
    //printf("Queue state before push: front=%p, back=%p\n", queue->front, queue->back);
    node_t* node = queue_create_node(data);
    if(!node)
    {
        return;
    }

    pthread_mutex_lock(&queue->lock);

    if(queue->back == NULL)
    {
        queue->back = node;
        queue->front = node;
    }
    else
    {
        queue->back->next = node; // Link the old back's next node to new node
        queue->back = node; // Assign the new back node to node
    }

    //printf("Pushed data: %p\n", data);
    //printf("Queue state after push: front=%p, back=%p, back->next=%p\n", queue->front, queue->back, queue->back->next);
    pthread_mutex_unlock(&queue->lock);
    pthread_cond_signal(&queue->notify);
}

void* queue_pop(queue_t* queue)
{
    //printf("Queue state before pop: front=%p, back=%p\n", queue->front, queue->back);
    pthread_mutex_lock(&queue->lock);

    while(queue->front == NULL) // Wait for new task
    {
        pthread_cond_wait(&queue->notify, &queue->lock);
    }

    node_t* old_node = queue->front;
    void* data = old_node->data;
    queue->front = queue->front->next;

    if(queue->front == NULL) // queue is now empty
    {
        queue->back = NULL;
    }

    //printf("Popped data: %p\n", data);
    //printf("Queue state after pop: front=%p, back=%p\n", queue->front, queue->back);
    pthread_mutex_unlock(&queue->lock);
    return data; // Return the node's data
}

void queue_destroy(queue_t* queue)
{
    pthread_mutex_lock(&queue->lock);
    node_t* current_node = queue->front;
    node_t* next_node;

    //printf("Queue state before free: front=%p, back=%p\n", queue->front, queue->back);
    
    while(current_node != NULL) 
    {
        next_node = current_node->next;
        //printf("Freeing node: %p\n", current_node->data);
        if(current_node->data != NULL)
        {
            free(current_node->data);
            //printf("data freed\n");
        }
        else
        {
            //printf("Warning: attempt to free a null ptr\n");
        }
        free(current_node);
        //printf("node freed\n");
        current_node = next_node; 
    }

    //printf("Queue state after free, front=%p, back=%p\n", queue->front, queue->back);
    pthread_mutex_unlock(&queue->lock);
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->notify);
    free(queue);
    //printf("Queue freed\n");
}
