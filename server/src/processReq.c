
#include "../include/processReq.h"

int responseNOK (int reqCode, int mapperID, void * arg) {
    struct threadArg * tArg = (struct threadArg *) arg;

    int response[RESPONSE_MSG_SIZE] = {reqCode, RSP_NOK, mapperID};
    //[-1] GET_MAPPER_UPDATES: 1 0
    if (mapperID == -1 && reqCode == GET_MAPPER_UPDATES) 
        response[2] = 0;
    write(tArg->clientFd, response, sizeof(response));
    return CHECKOUT;
}

int responseOK (int reqCode, int mapperID, void * arg) {
    struct threadArg * tArg = (struct threadArg *) arg;
    int response[RESPONSE_MSG_SIZE] = {reqCode, RSP_OK, mapperID};
    write(tArg->clientFd, response,  sizeof(response));
    return 0;
}

void * threadFunction(void * arg) {
	threadArg * tArg = (threadArg *) arg;
	int request[REQUEST_MSG_SIZE];
	
	if (read(tArg->clientFd, request, sizeof(request)) < 0) {
        perror("Error in read\n");
        pthread_exit(NULL);
    }

    int reqCode = request[0];
    int mapperID = request[1];

    // error cases
    if ((reqCode < 1 || reqCode > 6) || (mapperID <= 0 && mapperID != -1)){
        printf("---- Invalid request : %d, mapper PID : %d ----\n", reqCode, mapperID);
        responseNOK(reqCode, mapperID, tArg);
        if (close(tArg->clientFd) == -1) {
            perror("Error in close\n");
            pthread_exit(NULL);
        }
        printf("close connection from %s:%d\n", tArg->clientIp, tArg->clientPort);
        pthread_exit(NULL);
    }

    while (1) { 

        if (processReq(request, tArg) == CHECKOUT) {
            break;
        }

        if (read(tArg->clientFd, request, sizeof(request)) < 0) {
            perror("Error in read\n");
            pthread_exit(NULL);
        }

    } 
	if (close(tArg->clientFd) == -1) {
        perror("Error in close\n");
        pthread_exit(NULL);
    }

    printf("close connection from %s:%d\n", tArg->clientIp, tArg->clientPort);
	pthread_exit(NULL);
}


int processReq(int request[], void * arg) {
    struct threadArg * tArg = (struct threadArg *) arg;
    int reqCode = request[0];
    int mapperID = request[1];

    // 1. CHECKIN (Only Mapper clients can request)
    if (reqCode == CHECKIN) {
    
        printf("[%d] CHECKIN\n", mapperID);
        if (mapperID == -1) {
            printf("Mapper PID (%d) should be greater than 0\n", mapperID);
            return responseNOK(reqCode, mapperID, tArg);
        }
        //grab lock and process request
        pthread_mutex_lock(&lock);
        //corresponding entry does not exist and the CHECKIN is 0
        if (updateStatus[mapperID+1][0] == 0 && 
            updateStatus[mapperID+1][2] == 0) {
                updateStatus[mapperID+1][0] = mapperID;
                updateStatus[mapperID+1][2] = CHECKIN;
                pthread_mutex_unlock(&lock);
                return responseOK(reqCode, mapperID, tArg);
        }
        //exsit, and haven't checked in 
        if (updateStatus[mapperID+1][0] == mapperID &&
            updateStatus[mapperID+1][2] != CHECKIN) {
                updateStatus[mapperID+1][2] = CHECKIN;
                pthread_mutex_unlock(&lock);
                return responseOK(reqCode, mapperID, tArg);
        }
    }

    // 2. UPDATE_AZLIST (Only Mapper clients can request)
    if (reqCode == UPDATE_AZLIST) {
        // Server sums the word count results in the azList
        //grab lock and process request
        pthread_mutex_lock(&lock);
        if (updateStatus[mapperID+1][0] == mapperID) {
            for (int i = 0; i < 26; i++) {
                azList[i] += request[i+2]; // azList start from 0
            }
            // increases the number of update field of updateStatus table by 1 
            //for the corresponding mapper client. 
            updateStatus[mapperID+1][1]++;
            pthread_mutex_unlock(&lock);
            return responseOK(reqCode, mapperID, tArg);
        }
        else
            printf("[%d] UPDATE_AZLIST : Fail, haven't check in!\n", mapperID);
    }

    // 3. GET_AZLIST
    if (reqCode == GET_AZLIST) {
        //apper clients want to send this request to the server, 
        //they should be already checked in.
        //master clients can send it without check-in (extra credit)

        printf("[%d] GET_AZLIST\n", mapperID);
        //grab lock and process request
        pthread_mutex_lock(&lock);
        if (updateStatus[mapperID+1][2] == CHECKIN || mapperID == -1) {
            int response[LONG_RESPONSE_MSG_SIZE] = {0};
            response[0] = reqCode; response[1] = RSP_OK;
            for(int i = 0; i < 26; i++) {
                response[i+2] = azList[i];// azList is start from 0
            }
            write(tArg->clientFd, response, sizeof(response));
            pthread_mutex_unlock(&lock);

            // master client will only send one request each connection
            if (mapperID == -1)
                return CHECKOUT;
            return 0;
        }
        else
            printf("[%d] GET_AZLIST : Fail, haven't check in!\n", mapperID);
    }

    // 4. GET_MAPPER_UPDATES (Only Mapper clients can request)
    if (reqCode == GET_MAPPER_UPDATES) {
        // returns the current value of “number of updates” field of updateStatus table
        // for the corresponding mapper ID.

        printf("[%d] GET_MAPPER_UPDATES\n", mapperID);
        if (mapperID == -1) {
            printf("Mapper PID (%d) should be greater than 0\n", mapperID);
            return responseNOK(reqCode, mapperID, tArg);
        }

        pthread_mutex_lock(&lock);
        if (updateStatus[mapperID+1][0] == mapperID) {
            int response[RESPONSE_MSG_SIZE] = {0};
            response[0] = reqCode; response[1] = RSP_OK; 
            response[2] = updateStatus[mapperID+1][1];
            write(tArg->clientFd, response, sizeof(response));
            pthread_mutex_unlock(&lock);
            return 0;
        }
        else
            printf("[%d] GET_MAPPER_UPDATES : Fail, haven't check in!\n", mapperID);
    }

    // 5. GET_ALL_UPDATES 
    if (reqCode == GET_ALL_UPDATES) {
        //returns the sum of all values of “number of updates” field in the
        //updateStatus table.
        
        printf("[%d] GET_ALL_UPDATES\n", mapperID);
        pthread_mutex_lock(&lock); //master clients can send it without check-in 
        if (updateStatus[mapperID+1][0] == mapperID || mapperID == -1) {
            int response[RESPONSE_MSG_SIZE] = {0};
            for (int i = 0; i < MAX_STATUS_TABLE_LINES+2; i++) {
                response[2] += updateStatus[i][1];
            }
            response[0] = reqCode; response[1] = RSP_OK;
            write(tArg->clientFd, response, sizeof(response));
            pthread_mutex_unlock(&lock);

            // master client will only send one request each connection
            if (mapperID == -1)
                return CHECKOUT;
            return 0;
        }
        else
            printf("[%d] GET_ALL_UPDATES : Fail, haven't check in!\n", mapperID);
    }
 
    // 6. CHECKOUT (Only Mapper clients can request)
    if (reqCode == CHECKOUT) {
        //updates checkin/out field of the updateStatus table to checked-out(0)
        printf("[%d] CHECKOUT\n", mapperID);
        if (mapperID == -1) {
            printf("Mapper PID (%d) should be greater than 0\n", mapperID);
            return responseNOK(reqCode, mapperID, tArg);
        }

        pthread_mutex_lock(&lock);
        if (updateStatus[mapperID+1][0] == mapperID &&
            updateStatus[mapperID+1][2] == CHECKIN) {
                updateStatus[mapperID+1][2] = 0;
                pthread_mutex_unlock(&lock);
                responseOK(reqCode, mapperID, tArg);
                return CHECKOUT;
        }
        else
            printf("[%d] CHECKOUT : Fail, only CHECKIN can CHECKOUT\n", mapperID);
    }

    //Otherwiese error cases
    pthread_mutex_unlock(&lock);
    printf("---- Invalid request : %d, mapper PID : %d ----\n", reqCode, mapperID);
    return responseNOK(reqCode, mapperID, tArg);
}