#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <syslog.h>
#include "connector.h"

#define N 4

using namespace std;

sem_t* hostSemaphore;
sem_t* clientSemaphores[N];

const char SEMAPHORE_NAME[] = "/semaphore";
const char HOST_SEMAPHORE_NAME[] = "/hostSemaphore";

char answers[][20] = {"YES!", "NO", "I WANT TO SLEEP!", "WHO ARE YOU!?"};

void removeSemaphores()
{
	if (hostSemaphore != NULL && hostSemaphore != SEM_FAILED)
	{
		sem_close(hostSemaphore);
		sem_unlink(HOST_SEMAPHORE_NAME);
	}
	for(int i = 0; i < N;i++)
	{
		if(clientSemaphores[i] != NULL && clientSemaphores[i] != SEM_FAILED){
			sem_close(clientSemaphores[i]);
			sem_unlink(SEMAPHORE_NAME);
		}
	}
}

void initSemaphores()
{
    hostSemaphore = sem_open(HOST_SEMAPHORE_NAME, O_CREAT, S_IRWXU, 0);
	if(hostSemaphore == SEM_FAILED)
    {
        syslog(LOG_INFO, "can't init host semaphore");
    }

    for(int i = 0; i < N; i++)
    {
        clientSemaphores[i] = sem_open(SEMAPHORE_NAME, O_CREAT, S_IRWXU, 0);
        if(clientSemaphores[i] == SEM_FAILED)
        {
            syslog(LOG_INFO, "can't init client semaphore");
        }
    }
}

void runHost()
{
    int i, ans;
    char str[PATH_MAX];
    while(true)
    {
        gets(str);
        for(i=0; i < N; i++){
            if (write(&str, PATH_MAX*sizeof(char)) == 0)
                syslog(LOG_INFO, "no connection");

            sem_post(clientSemaphores[i]);
            sem_wait(hostSemaphore);

            if(read(&str, PATH_MAX*sizeof(int)) == 0)
                syslog(LOG_INFO, "no connection");
            cout << "answer from " << i << " expert is: " << str << "\n";
        }
    }
	return;
}

void runClient(int i)
{
	int answer;
	char str[PATH_MAX];
    char strAns[PATH_MAX];
	srand(i);
	while(true)
	{
	    sem_wait(clientSemaphores[i]);
        if (read(str, PATH_MAX*sizeof(char)) == 0)
            syslog(LOG_INFO, "no connection");

        answer = rand() % 4;
        strcpy(strAns, answers[answer]);
	    if (write(&strAns, PATH_MAX*sizeof(int)) == 0)
            syslog(LOG_INFO, "no connection");
	    sem_post(hostSemaphore);
    }
	return;
}
int main(void)
{
	createConnection();
    initSemaphores();

    for(int i = 0; i < N; i++)
    {
        int pid = fork();
        if(pid == 0)
        {
            syslog(LOG_INFO, "forked");
            runClient(i);
            continue;
        }
        else if (pid < 0) {
            syslog(LOG_INFO, "fork error");
            exit(EXIT_FAILURE);
        }
    }

    runHost();
    removeSemaphores();
    if(haveConnection())
    {
        syslog(LOG_INFO, "destroy connection");
		destroyConnection();
    }

    return 0;
}
