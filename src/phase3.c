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
