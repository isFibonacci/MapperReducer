#ifndef PROCESSREQ_H
#define PROCESSREQ_H

#define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

//use Lab13
typedef struct threadArg{
	int clientFd;
	char * clientIp;
	int clientPort;
} threadArg;

extern int azList[ALPHABETSIZE];
// plus two for extra credit, row 1 is empty: see CHECKIN
extern int updateStatus[MAX_STATUS_TABLE_LINES+2][3];

extern int totalClients;
extern pthread_mutex_t lock;

int responseNOK (int reqCode, int mapperID, void * arg);
int responseOK (int reqCode, int mapperID, void * arg);

void * threadFunction(void * arg);

int processReq(int request[], void * arg);

#endif

