#include "phase3.h"
#include "phase1.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

// You are free to use your own logic. The following points are just for getting started
/* Reduce Function
	1)	The reducer receives the list from 'm' mappers via pipes
	2)	Ensure proper closing of pipe ends
	3) 	Combine the lists to create a single list
	4) 	Write the list to "ReducerResult.txt" in the current folder
*/

int reduce(int *temp) {
	for (int i = 0; i < 26; i++){
		list[i] += temp[i];
	}
}

int reduceResult() {
	char *buf = "./ReducerResult.txt";

	FILE *fp = fopen(buf, "a");
    if (fp == NULL) {
        perror("Error opening file!\n");
		return 0;
    }
	int i = 0;
	for(int c = 'A'; c <= 'Z'; c++) {
		fprintf(fp, "%c %d\n", c, list[i]);
		i += 1;
	}
	fclose(fp);
    return 1;
}