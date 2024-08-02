#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "queue.h"

typedef struct
{
    void* arg1;
    void* arg2;
    void* arg3;
} threadpool_args_t;

threadpool_args_t* threadpool_create_args(void* arg1, void* arg2, void* arg3);

typedef struct
{
    void (*function)(void*);
    threadpool_args_t* args;
} threadpool_task_t;

threadpool_task_t* threadpool_create_task(void (*function)(void*), threadpool_args_t* args);

typedef struct
{
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t* threads;
    queue_t* queue;
    int thread_count;
    int queue_size;
    int shutdown;
} threadpool_t;

threadpool_t* threadpool_create(int thread_count);
int threadpool_push_task(threadpool_t* threadpool, threadpool_task_t* task);
void* _threadpool_worker(void* threadpool);
int threadpool_destroy(threadpool_t* threadpool);