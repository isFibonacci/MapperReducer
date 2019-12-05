#include "phase1.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int checkEmptyDir(char *path) {
	DIR *dir = NULL;
	struct dirent *entry;
 
     // if the folder is empty or not exists, return 1. Otherwise, return 0. 

	dir = opendir(path);
	if (dir == NULL) {
		fprintf(stderr,"The %s does not exists!\n", path);
		return 1;
	}
 
	while ((entry = readdir(dir))) {
		if((strcmp(entry->d_name, ".") == 0) ||
		    (strcmp(entry->d_name, "..") == 0)) 
		    continue;
		closedir(dir);
		return 0;
	}
 
	closedir(dir);
	fprintf(stderr,"The %s folder is empty\n", path);
	return 1;
}

void createInputFolder() {
	struct stat st = {0};

	// Check if "MapperInput" already exists
	if (stat("./MapperInput", &st) == -1) {
		mkdir("./MapperInput", 0700);
	}
	fileCount = 0;
}

int processDirectory(char *theDir , int mapperCount) {
	DIR *dir = NULL;
	struct dirent *entry;
	char pathName[256];
    
	/* Open the given directory, if we can. */	
	dir = opendir(theDir);
	if(dir == NULL) {
		perror( "Error in process_directory");
		return 0;
	}
	
	while(entry = readdir(dir)) {
		struct stat entryInfo;
		
		if((strncmp(entry->d_name, ".", 256) == 0) ||
		    (strncmp(entry->d_name, "..", 256) == 0)) 
		    continue;
	
		strncpy(pathName, theDir, 256 );
		strncat(pathName, "/", 256 );
		strncat(pathName, entry->d_name, 256);
		
		if(lstat(pathName, &entryInfo) == 0) {

			if(S_ISDIR(entryInfo.st_mode)) {		
				processDirectory(pathName, mapperCount);
			} 
            else if(S_ISREG(entryInfo.st_mode)) {
				int m = fileCount % mapperCount;
                writePathToMappers(m, pathName);
                fileCount += 1;
			}
		} 
        else {
			perror( "Error in process_directory");
			return 0;
		}
	}
	closedir(dir);
}

int writePathToMappers(int m, char *filePath) {
	char num[5];
	sprintf(num, "%d", m);
	char *front = "./MapperInput/Mapper_";
	char *end = ".txt";
	char buf[64];

	// contat mapper file name
	strcpy(buf, front);
	strcat(buf, num);
	strcat(buf, end);

    FILE *fp = fopen(buf, "a");
    if (fp == NULL) {
        perror("Error opening file!\n");
		return 0;
    }

    fprintf(fp, "%s\n", filePath);
    fclose(fp);
    return 1;
}
