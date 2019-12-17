
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#include "../include/phase1.h"

void recursiveTraverseFS(int mappers, char *basePath, FILE *fp[], int *toInsert, int *nFiles){
	//check if the directory exists
	DIR *dir = opendir(basePath);
	if(dir == DIRNULL){
		printf("Unable to read directory %s\n", basePath);
		exit(1);
	}

	char path[1000];
	struct dirent *dirContentPtr;

	//use https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
	while((dirContentPtr = readdir(dir)) != DIRNULL){
		if (strcmp(dirContentPtr->d_name, ".") != 0 && 
			strcmp(dirContentPtr->d_name, "..") != 0 &&
			strcmp(dirContentPtr->d_name, ".DS_Store") != 0){
			struct stat buf;
			 lstat(path, &buf);
			 if (S_ISLNK(buf.st_mode))
				 continue;
			if (dirContentPtr->d_type == DT_REG){
				//file
				char filePath[1000];
				strcpy(filePath, basePath);
				strcat(filePath, "/");
				strcat(filePath, dirContentPtr->d_name);
				
				//insert into the required mapper file
				fputs(filePath, fp[*toInsert]);
				fputs("\n", fp[*toInsert]);

				*nFiles = *nFiles + 1;
				//change the toInsert to the next mapper file
				*toInsert = (*toInsert + 1) % mappers;
			}else if (dirContentPtr->d_type == DT_DIR){
				//directory
				// basePath creation - Linux platform
				strcpy(path, basePath);
				strcat(path, "/");
				strcat(path, dirContentPtr->d_name);
				recursiveTraverseFS(mappers, path, fp, toInsert, nFiles);
			}
		}
	}
}

void traverseFS(int mappers, char *path){
    FILE *fp[mappers];

    pid_t p = fork();
    if (p==0)
        execl("/bin/rm", "rm", "-rf", "MapperInput", NULL);

    wait(NULL);
    //Create a folder 'MapperInput' to store Mapper Input Files
    mkdir("MapperInput", ACCESSPERMS);

    // open mapper input files to store paths of files to be processed by each mapper
    int i;
    for (i = 0; i < mappers; i++){
        // create the mapper file name
        char mapperCount[10];
        sprintf(mapperCount, "%d", i + 1);
        char mapInFileName[100] = "MapperInput/Mapper_";
        strcat(mapInFileName, mapperCount);
        strcat(mapInFileName, ".txt");
        fp[i] = fopen(mapInFileName, "w");

    }
    //refers to the File to which the current file path should be inserted
    int toInsert = 0;
    int nFiles = 0;
    recursiveTraverseFS(mappers, path, fp, &toInsert, &nFiles);

    // close all the file pointers
    for(i = 0; i < mappers; i++)
        fclose(fp[i]);

    if(nFiles == 0){
        pid_t p1 = fork();
        if (p1==0)
            execl("/bin/rm", "rm", "-rf", "MapperInput", NULL);
    }

    if (nFiles == 0){
        printf("The ../%s folder is empty\n", path);
        exit(0);
    }
}
