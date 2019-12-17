#ifndef PHASE2_H
#define PHASE2_H

#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

void checkIn(int sockfd, FILE *logfp, int mapperId);

void updateAzlist(int sockfd, FILE *logfp, int mapperId);

void getAzlist(int sockfd, FILE *logfp, int mapperId);

void getMapperUpdates(int sockfd, FILE *logfp, int mapperId);

void getAllUpdates(int sockfd, FILE *logfp, int mapperId);

void checkOut(int sockfd, FILE *logfp, int mapperId);

#endif