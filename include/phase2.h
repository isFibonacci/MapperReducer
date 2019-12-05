// Add Guard to header file
#ifndef __PHASE_TWO_H
#define __PHASE__TWO_H

#define MAX 26

int Mapper(char *fileName, int*temp);

int *openFile(char *MapperTxt);

int creataMmapperList();

char *mapperList[32];

int alphabet[MAX];

#endif