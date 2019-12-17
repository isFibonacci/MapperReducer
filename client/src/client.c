
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/protocol.h"

FILE *logfp;

void createLogFile(void) {
    pid_t p = fork();
    if (p==0)
        execl("/bin/rm", "rm", "-rf", "log", NULL);

    wait(NULL);
    mkdir("log", ACCESSPERMS);
    logfp = fopen("log/log_client.txt", "w");
}

int main(int argc, char *argv[]) {
    int mappers;
    char folderName[100] = {'\0'};
    char *server_ip;
    int server_port;

    if (argc == 5) { // 4 arguments
        strcpy(folderName, argv[1]);
        mappers = atoi(argv[2]);
        server_ip = argv[3];
        server_port = atoi(argv[4]);
        if (mappers > MAX_MAPPER_PER_MASTER) {
            printf("Maximum number of mappers is %d.\n", MAX_MAPPER_PER_MASTER);
            printf("./client <Folder Name> <# of mappers> <server IP> <server Port>\n");
            exit(1);
        }

    } else {
        printf("Invalid or less number of arguments provided\n");
        printf("./client <Folder Name> <# of mappers> <server IP> <server Port>\n");
        exit(1);
    }

    // create log file
    createLogFile();

    // phase1 - File Path Partitioning
    traverseFS(mappers, folderName);

    // Phase2 - Mapper Clients's Deterministic Request Handling
    pid_t childpid;
    int i  = 0;

    printf("Mapper clients start...\n");    sleep(1); printf("Wait mapper clients...\n");
	for (; i < mappers; i++){
        if ((childpid = fork()) == 0) {
            // Create a TCP socket.
            int sockfd = socket(AF_INET , SOCK_STREAM , 0);

            // Specify an address to connect to (we use the local host or 'loop-back' address).
            struct sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_port = htons(server_port);
            address.sin_addr.s_addr = inet_addr(server_ip);

            // Connect it.
            if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0) {
                fprintf(logfp, "[%d] open connection\n", i + 1);
                
                // 1. CHECKIN
                checkIn(sockfd, logfp, i+1);

                // 2. UPDATE_AZLIST
                updateAzlist(sockfd, logfp, i+1);

                // 3. GET_AZLIST
                getAzlist(sockfd, logfp, i+1);

                // 4. GET_MAPPER_UPDATES
                getMapperUpdates(sockfd, logfp, i+1);

                // 5. GET_ALL_UPDATES
                getAllUpdates(sockfd, logfp, i+1);

                // 6. CHECKOUT
                checkOut(sockfd, logfp, i+1);

                //close connection
                close(sockfd);

                fprintf(logfp, "[%d] close connection\n", i+1);

                //mapper client exit
                return 0;
               
            }
            else {
                perror("Connection failed!");
                return -1;
            }

		}
		else if ((childpid == -1) && (errno != EINTR)){
				perror("Error in spwaning mappers\n");
				return -1;
		}
	}
    sleep(2);   wait(NULL);     printf("Mapper clients done!\n");   
    // Phase3 - Master Client's Dynamic Request Handling (Extra Credit)
    int masterClient = -1;

    //open commands.txt
    char* commandsFile = "commands.txt";

    FILE *commandsFilePtr;
    if((commandsFilePtr = fopen(commandsFile, "r")) == NULL) {
        perror("commandsFile cannot be open!\n");
        return -1;
    }

    char charCommand[4];
    sleep(1);   printf("Master client start...\n");
    while (fgets(charCommand, 4, commandsFilePtr) != NULL) {

        // remove newline character and convert command to int
        charCommand[strcspn(charCommand, "\n")] = 0;
        int intCommand = atoi(charCommand);
    
        //if wrong command, next loop
        if (intCommand < 1 || intCommand > 6 || intCommand == 2 || strlen(charCommand) > 1) {
            fprintf(logfp, "[%d] wrong command\n", masterClient);
            memset(charCommand, 0, sizeof(charCommand));
            continue;
        }
        memset(charCommand, 0, sizeof(charCommand));

        // Create a TCP socket.
        int sockfd = socket(AF_INET , SOCK_STREAM , 0);

        // Specify an address to connect to (we use the local host or 'loop-back' address).
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(server_port);
        address.sin_addr.s_addr = inet_addr(server_ip);


        // Connect it.
        if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0) {
                fprintf(logfp, "[%d] open connection\n", masterClient);

                // 1. CHECKIN
                if (intCommand == CHECKIN)
                    checkIn(sockfd, logfp, masterClient);

                // 3. GET_AZLIST
                if (intCommand == GET_AZLIST)
                    getAzlist(sockfd, logfp, masterClient);

                // 4. GET_MAPPER_UPDATES
                if (intCommand == GET_MAPPER_UPDATES)
                    getMapperUpdates(sockfd, logfp, masterClient);

                // 5. GET_ALL_UPDATES
                if (intCommand == GET_ALL_UPDATES)
                    getAllUpdates(sockfd, logfp, masterClient);

                // 6. CHECKOUT
                if (intCommand == CHECKOUT)
                    checkOut(sockfd, logfp, masterClient);

                //close connection
                close(sockfd);
                fprintf(logfp, "[%d] close connection\n", masterClient);
               
        }
        else {
                perror("Connection failed!");
                return -1;
        }

    }
    fclose(logfp);
    sleep(1);
    printf("Master client done!\n");
    return 0;

}
