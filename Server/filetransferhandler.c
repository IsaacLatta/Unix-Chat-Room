#include "filetransferhandler.h"

void init_handler(FileTransferHandler_t* handler, int port, int max_clients)
{
    int opt = 1;
    handler->tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(handler->tcp_sock < 0)
    {
        exit_on_error("[-] tcp socket() failed");
    }

    if(setsockopt(handler->tcp_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) )
    {
        exit_on_error("[-] setsockopt() failed on tcp socket");
    }

    handler->addr.sin_family = AF_INET;
    handler->addr.sin_port = htons(port);
    handler->addr.sin_addr.s_addr = INADDR_ANY;
    handler->addr_len = sizeof(handler->addr);

    if(bind(handler->tcp_sock, (struct sockaddr*)&handler->addr, handler->addr_len) < 0)
    {
        exit_on_error("[-] bind() failed on tcp socket");
    }
    if(listen(handler->tcp_sock, max_clients) < 0)
    {
        exit_on_error("[-] listen() failed on tcp socket");
    }

    int is_running = 1;
    // init_threadpool()
}

void accept_connection(FileTransferHandler_t* handler)
{
    ft_request_t* request = (ft_request_t*)ec_malloc(sizeof(ft_request_t));
    request->client_addr_len = sizeof(request->client_addr);
    int new_sock = accept(handler->tcp_sock, (struct sockaddr*)&request->client_addr, &request->client_addr_len);
    if(new_sock < 0)
    {
        perror("[-] accept() failed");
        free(request);
        close(new_sock); 
        return;
    }
    request->sock = new_sock;

    // push task to thread pool

    free(request);
    close(new_sock); 
}

void stop_handler(FileTransferHandler_t* handler)
{
    handler->is_running = 0;
    // Wait for thread pool
    close(handler->tcp_sock);
}

void* get_connections(void* arg)
{
    FileTransferHandler_t* handler = (FileTransferHandler_t*) arg;
    struct pollfd fds[1];
    fds[0].fd = handler->tcp_sock;
    fds[0].events = POLLIN; 

    while(handler->is_running)
    {
        int res = poll(fds, 1, 500); // Wait 500ms
        if(res < 0)
        {
            perror("[-] poll() error in ft_handler");
            stop_handler(handler);
            break;
        }

        if(fds[0].revents & POLLIN)
        {
            accept_connection(handler);
        }
    }
    return NULL;
}

void run_handler(FileTransferHandler_t* handler)
{
    int res = pthread_create(&handler->thread_id, NULL, get_connections, handler);
    if(res != 0)
    {
        exit_on_error("[-] pthread_create() failed on launching ft_handler");
    }
    printf("[+] ft_handler is up\n");
}
