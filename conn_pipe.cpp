#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "connector.h"

int fileDesc[2];
int connected = false;

void createConnection()
{
	if(pipe(fileDesc) != -1)
		connected = true;
	else
		connected = false;
}

int read(void* data, size_t size)
{
	if(read(fileDesc[0], data, size) == -1)
		return 0;
	else
		return 1;
}

int haveConnection()
{
    return connected;
}

int write(void* data, size_t size)
{
	if(write(fileDesc[1], data, size) == -1)
		return 0;
	else
		return 1;
}

void destroyConnection()
{
    close(fileDesc[0]);
    close(fileDesc[1]);
}

