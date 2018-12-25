#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include "connector.h"

#define BUFFER_SIZE 4

void* buffer = NULL;
int connected = false;

void createConnection()
{
    buffer = (void*)mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (buffer == MAP_FAILED)
        buffer = NULL;
    else
        connected = true;
}

void destroyConnection()
{
    connected = false;
    munmap(buffer, BUFFER_SIZE);
}

int haveConnection()
{
    return connected;
}

int read(void* data, size_t size)
{
    if(size > BUFFER_SIZE)
        return 0;
    memcpy(data, buffer, BUFFER_SIZE);
    return 1;
}

int write(void* data, size_t size)
{
    if(size > BUFFER_SIZE)
        return 0;
    memcpy(buffer, data, BUFFER_SIZE);
    return 1;
}


