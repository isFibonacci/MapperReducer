
#define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include "../include/phase2.h"

void checkIn(int sockfd, FILE *logfp, int mapperId) {
    
    // send request
    int request[28] ={0};
    //memset(request, 0, sizeof(request));
    request[0] = 1;  // 1 for CHECKIN
    request[1] = mapperId;         
    write(sockfd, request, sizeof(request));

    // get response
    int response[3] = {0};
    if (read(sockfd, response, sizeof(response)) < 0 ) {
        perror("Error in read!\n");
        exit(0);
    }

    fprintf(logfp, "[%d] CHEKCIN: %d %d\n", mapperId, response[1], response[2]);

}

void updateAzlist(int sockfd, FILE *logfp, int mapperId) {
    char mapperCount[10];
    memset(mapperCount, 0, sizeof(mapperCount));

    sprintf(mapperCount, "%d", mapperId);
    char mapInFileName[256] = "MapperInput/Mapper_";
    strcat(mapInFileName, mapperCount);
    strcat(mapInFileName, ".txt");

    // oepn mapper_mapperId.txt in MapperInput
    FILE *mapperTxt;
    if((mapperTxt = fopen(mapInFileName, "r")) == NULL) {
        perror("file cannot be open!\n");
        exit(0);
    }

    char line[256];
    int msgCount = 0;

    while (fgets(line, 256, mapperTxt) != NULL) {
    
        // remove newline character
        line[strcspn(line, "\n")] = 0;

        // check and open file
        FILE* filePointer = fopen(line, "r");
        if (filePointer == NULL) {
            perror("Can't open file!");
            exit(0);
        }
        int request[28] = {0};
        char buff[256];
        memset(buff, 0, sizeof(buff));

        int lineNum = 0;
        // deal each line in that file
        while(fgets(buff, 256, filePointer)) {
            
            lineNum += 1;
            char * oneLine = NULL;
            int lineLength = strlen(buff)-1;
            oneLine = (char*)calloc(lineLength, sizeof(char));
            strncpy(oneLine, buff, lineLength);
            char c = tolower(oneLine[0]);
            if(c >= 'a' && c <= 'z')
                request[2 + c - 'a'] += 1;
            free(oneLine);
            oneLine = NULL;
            memset(buff, 0, sizeof(buff));
        }

        fclose(filePointer);
        memset(line, 0, sizeof(line));
        //If there is no files in the mapper file, next loop
        if (lineNum == 0)
            continue;

        // send request to server
        request[0] = 2;     // 2 for UPDATE_AZLIST
        request[1] = mapperId;
    
        write(sockfd, request, sizeof(request));
        msgCount += 1;
        // get response from server
        int response[3] = {0};
        if (read(sockfd, response, sizeof(response)) < 0 ) {
            perror("Error in read!\n");
            exit(0);
        }

    }

    fclose(mapperTxt);
    fprintf(logfp,"[%d] UPDATE_AZLIST: %d\n", mapperId, msgCount);
}

void getAzlist(int sockfd, FILE *logfp, int mapperId) {

    int request[28];
    memset(request, 0, sizeof(request));
    request[0] = 3;     // 3 for GET_AZLIST
    request[1] = mapperId;

    // send request to server
    write(sockfd, request, sizeof(request));

    // get response from server
    int response[28] = {0};
    if (read(sockfd, response, sizeof(response)) < 0 ) {
        perror("Error in read!\n");
        exit(0);
    }
    
    // print log
    fprintf(logfp, "[%d] GET_AZLIST: %d ", mapperId, response[1]);
    for(int i = 2; i < 28; i++) {
        fprintf(logfp, "%d ", response[i]); 
    }
    fprintf(logfp, "\n");
}

void getMapperUpdates(int sockfd, FILE *logfp, int mapperId) {

    int request[28];
    memset(request, 0, sizeof(request));
    request[0] = 4;     // 4 for GET_MAPPER_UPDATES
    request[1] = mapperId;

    // send request to server
    write(sockfd, request, sizeof(request));

    // get response from server
    int response[3];
    if (read(sockfd, response, sizeof(response)) < 0 ) {
        perror("Error in read!\n");
        exit(0);
    }

    // print log
    fprintf(logfp, "[%d] GET_MAPPER_UPDATES: %d %d\n", mapperId, response[1], response[2]);

}

void getAllUpdates(int sockfd, FILE *logfp, int mapperId) {
    
    int request[28];
    memset(request, 0, sizeof(request));
    request[0] = 5;     // 5 for GET_ALL_UPDATES
    request[1] = mapperId;

    // send request to server
    write(sockfd, request, sizeof(request));

    // get response from server
    int response[3];
    if (read(sockfd, response, sizeof(response)) < 0 ) {
        perror("Error in read!\n");
        exit(0);
    }

    // print log
    fprintf(logfp, "[%d] GET_ALL_UPDATES: %d %d\n", mapperId, response[1], response[2]);
}

void checkOut(int sockfd, FILE *logfp, int mapperId) {
    
    int request[28];
    memset(request, 0, sizeof(request));
    request[0] = 6;     // 6 for CHECKOUT
    request[1] = mapperId;

    // send request to server
    write(sockfd, request, sizeof(request));

    // get response from server
    int response[3];
    if (read(sockfd, response, sizeof(response)) < 0 ) {
        perror("Error in read!\n");
        exit(0);
    }

    // print log
    fprintf(logfp, "[%d] CHECKOUT: %d %d\n", mapperId, response[1], response[2]);
}
