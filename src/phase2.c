#include "../include/phase2.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

int *openFile(char *d_name) {
	
	FILE* fp = fopen(d_name, "r");

	// check and open file
	if (fp == NULL) {
		perror("Error in open file at phase2");
		return 0;
	}
	
	char buff[256];
	int *temp = (int*)calloc(sizeof(int),26);

	while(fgets(buff, 256, fp)) {
	
		//The last element in line is LF means new line, so don't count in
		int lineLength = strlen(buff)-1;

		//each line is a path of txt
		char * pathOftxt;
		pathOftxt = (char*)calloc(sizeof(char), lineLength + 1);
		strncpy(pathOftxt, buff, lineLength);
		Mapper(pathOftxt, temp);
		free(pathOftxt);
		pathOftxt = NULL;
		memset(&buff[0], 0, sizeof(buff));
	}

	fclose(fp);
	return temp;
}

int Mapper(char *fileName, int *temp){
	
	FILE* filePointer = fopen(fileName, "r");

	// check and open file
 	if (!filePointer) {
		perror("Error ");
		return -1;
	}
	char buff[256];
	
	// deal each line in that file
	while (fgets(buff, 256, filePointer)){
		char * oneLine;
		int lineLength = strlen(buff)-1;
		oneLine = (char*)calloc(lineLength, sizeof(char));
		strncpy(oneLine, buff, lineLength);
		char c = tolower(oneLine[0]);
		if(c >= 'a' && c <= 'z')
			temp[c - 'a'] += 1;
		free(oneLine);
		oneLine = NULL;
		memset(&buff[0], 0, sizeof(buff));
	}

	fclose(filePointer);
    return 1;
}

int creataMmapperList(){

	char * folder = "./MapperInput";
	DIR *dir = opendir(folder);
	struct  dirent *di;
	int i = 0;
	while(di = readdir(dir)) {
		if (strcmp(di->d_name, ".") == 0 || strcmp(di->d_name, "..") == 0) {
			continue;
		}
		else if (di->d_type == 8) {
			mapperList[i] = (char*)calloc(sizeof(char), (strlen(folder)+strlen(di->d_name)+2));
			strncpy(mapperList[i], folder, strlen(folder));
			strcat(mapperList[i], "/");
			if (strlen(di->d_name) > 100)
				perror("di->d_name > 100");
			strncat(mapperList[i], di->d_name, strlen(di->d_name));
		}
		i += 1;
	}
	if (i == 0) {
		closedir(dir);
		return 0;
	}
	closedir(dir);
	return 1;	
}
