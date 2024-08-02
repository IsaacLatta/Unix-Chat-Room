#include "util.h"

void* ec_malloc(int size)
{
    void* buffer = malloc(size);
    if(!buffer)
    {
        perror("malloc() failed");
        exit(EXIT_FAILURE);
    }
    return buffer;
}

void exit_on_error(char* error_msg)
{
    perror(error_msg);
    exit(EXIT_FAILURE);
}

char* get_files(char* path) 
{
    int buffer_size = 1024;
    char* buffer = (char*)ec_malloc(buffer_size);
    memset(buffer, 0, buffer_size);
    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("[-] opendir() failed");
        return NULL;
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char file_path[256];
        snprintf(file_path, sizeof(file_path), "./files/%s", entry->d_name);
        struct stat file_stat;
        if(stat(file_path, &file_stat) != 0)
        {
            perror("[-] stat() failed");
            continue;
        }

        char file_info[256];
        snprintf(file_info, sizeof(file_info), "%s (%ld bytes)\n", 
        entry->d_name, file_stat.st_size);
        strncat(buffer, file_info,  buffer_size - strlen(buffer) - 1);
    }
    
    closedir(dir);
    return buffer;
}
