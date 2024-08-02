#include "threadpool.h"

threadpool_args_t* threadpool_create_args(void* arg1, void* arg2, void* arg3)
{
    threadpool_args_t* args = (threadpool_args_t*)malloc(sizeof(threadpool_args_t));
    if(!args)
    {
        return NULL;
    }
    args->arg1 = arg1;
    args->arg2 = arg2;
    args->arg3 = arg3;
    return args;
}

threadpool_task_t* threadpool_create_task(void (*function)(void*), threadpool_args_t* args)
{
    threadpool_task_t* task = (threadpool_task_t*)malloc(sizeof(threadpool_task_t));
    if(!task)
    {
        return NULL;
    }
    task->function = function;
    task->args = args;
    return task;
}

threadpool_t* threadpool_create(int thread_count)
{
    threadpool_t* pool = (threadpool_t*)malloc(sizeof(threadpool_t));
    if(pool == NULL)
    {
        return NULL;
    }

    if(pthread_mutex_init(&pool->lock, NULL) != 0)
    {
        free(pool);
        return NULL;
    }
    if(pthread_cond_init(&pool->notify, NULL) != 0)
    {
        pthread_mutex_destroy(&pool->lock);
        free(pool);
        return NULL;
    }
    pool->threads = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
    if(pool->threads == NULL)
    {
        pthread_cond_destroy(&pool->notify);
        pthread_mutex_destroy(&pool->lock);
        free(pool);
        return NULL;
    }
    pool->queue = queue_create();
    if(pool->queue == NULL)
    {
        pthread_cond_destroy(&pool->notify);
        pthread_mutex_destroy(&pool->lock);
        free(pool->threads);
        free(pool);
        return NULL;
    }

    pool->thread_count = thread_count;
    pool->shutdown = 0;

    for(int i = 0; i < thread_count; i++)
    {
        if(pthread_create(&pool->threads[i], NULL, _threadpool_worker, (void*)pool) != 0)
        {
            threadpool_destroy(pool);
            return NULL;
        }
    }
    return pool;
}