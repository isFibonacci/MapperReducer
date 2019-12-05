#include "phase4.h"

#include "../include/phase2.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int redirect(int mapperCount) {
	int fd;
	if( (fd = open("./FinalResult.txt", CREATE_FLAGS, CREATE_MODE)) == -1){
		perror("Failed to open ./FinalResult.txt"); 
		return 0;
	}

	//copy fd file entry into stand output
	if(dup2(fd, STDOUT_FILENO) == -1) {
		perror("Failed to redirect ./FinalResult.txt");
		return 0;
	}

	//close the old file entry
	if (close(fd) == -1 ) {
		close(fd);
		perror("Error in clsoe file");
		return 0;
	}

	FILE *fp = fopen("./ReducerResult.txt", "r");
	if (fp == NULL) {
		perror("Error in open file at phase4");
		return 0;
	}
	
	char buff[10];
	while(fgets(buff, 10, fp)) {
		printf("%s", buff);
	}
	fclose(fp);

	for(int i = 0; i < mapperCount; i++) {
		free(mapperList[i]);
	}
	return 1;
}