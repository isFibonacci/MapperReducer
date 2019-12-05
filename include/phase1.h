// Add Guard to header file (google what are guards)
#ifndef __PHASE_ONE_H
#define __PHASE_ONE_H

int fileCount;
extern int fileCount;

void createInputFolder();

int processDirectory( char *theDir , int mapperCount);

int writePathToMappers(int m, char *filePath);

int checkEmptyDir(char *path);

#endif