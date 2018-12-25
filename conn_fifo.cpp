#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "connector.h"


#define FIFO "./fifo_file"

int connected = false;
int fdr= 0;
int fdw = 0;

void createConnection()
{
    if(mkfifo(FIFO, S_IRWXU) == 0)
        connected = true;
    else
        connected = false;

    if (!connected)
        return;
    fdr = open(FIFO, O_RDONLY | O_NONBLOCK);
    fdw = open(FIFO, O_WRONLY | O_NONBLOCK);
    if(fdr != -1 && fdw != -1)
        connected = true;
    else
        connected = false;
  }

void destroyConnection()
{
    connected = false;
    close(fdr);
    close(fdw);
    unlink(FIFO);
}

int haveConnection()
{
    return connected;
}

int read(void* data, size_t size)
{
    if(read(fdr, data, size) == -1)
        return 0;
    else
        return 1;
}

int write(void* data, size_t size)
{
    if(write(fdw, data, size) == -1)
        return 0;
    else
        return 1;
}
