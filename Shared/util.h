#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void* ec_malloc(int size);
void exit_on_error(char* error_msg);
char* get_files(char* dir); // produce a formatted string of available files in specified dir