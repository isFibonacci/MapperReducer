
#include "../include/main.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	// argument count check
	if (argc != 3) {
		printf("Please enter 2 arguments to execute!");
		return -1;
	}
	
	// phase1 - Data Partition Phase
	// e.g: test TestCase2: ./mapreduce ../Testcases/TestCase2 1

	int mapperCount = atoi(argv[2]);

	// Check if the input folder exits
	if (checkEmptyDir(argv[1])) 
		exit(0);

	createInputFolder();
	processDirectory(argv[1], mapperCount);

	if (creataMmapperList() != 1){
		perror("Empty file");
		return 0;
	}

	//create pipes
	int filePipes[mapperCount][2];
	for ( int i = 0; i < mapperCount; i++ ){
		if (pipe(filePipes[i]) == -1){
			perror("Fail to create the pipe");
			return -1;
		}
	}

	// phase2 - Mapper 
	int i = 0;
	pid_t childpid;
	for (i = 0; i < mapperCount; i++){
		if ( (childpid = fork()) > 0){
			wait(NULL);
			if(i == 0)
				break;
			else 
				exit(0);
		}
		else if (childpid == 0) {
			close(filePipes[i][0]);
			int *temp = openFile(mapperList[i]);
			if(write(filePipes[i][1],temp, sizeof(int)*26) == 0)
				perror("Error in write");
			close(filePipes[i][1]);
		}
		else if ((childpid == -1) && (errno != EINTR)){
				perror("Error in rewrite");
				return 0;
		}
	}

	// phase3 - Reduce Function
	if( i == mapperCount)// the last child
		exit(0);

	pid_t pid = fork();
	if (pid == 0) {
		for(int i = 0; i < mapperCount; i++){
			close(filePipes[i][1]);
			int *result = (int*)malloc(sizeof(int)*26);
			if(read(filePipes[i][0], result, sizeof(int)*26) == 0)
				perror("Error in reread");
			reduce(result);
			close(filePipes[i][0]);
			free(result);
		}
		reduceResult();
		exit(0);
	}

	//phase4
	//wait for all processes to reach this point
	else if(pid > 0)
		wait(NULL);

	//just make a function call to code in phase4.c
	//master process reports the final output
	redirect(mapperCount);


	return 0;

}
