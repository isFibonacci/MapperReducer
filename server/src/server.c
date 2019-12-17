
#include "../include/processReq.h"


int totalClients = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int azList[ALPHABETSIZE] = {0};
// plus two for extra credit, row 1 is empty: see CHECKIN
int updateStatus[MAX_STATUS_TABLE_LINES+2][3] = {0};

int server(int server_port) {
    pthread_t threads[MAX_CONCURRENT_CLIENTS];
	int count = 0;
	pthread_mutex_init(&lock, NULL);

    // Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
        return -1;

    //clean port
    int optval = 1; 
    int r = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (r != 0) {
        printf("Cannot enable SO_REUSEADDR option.\n");
        return -1;
    }

    // Bind it to a local address. 
	struct sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(server_port);
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *) &servAddress, sizeof(servAddress)) == -1)
        return -1;

    //Listen on this port and block
	if (listen(sock, MAX_CONCURRENT_CLIENTS) == -1)
        return -1;

    printf("server is listening\n");

    while (1) {
        //accept the incoming connections.
		struct sockaddr_in clientAddress;

		socklen_t size = sizeof(struct sockaddr_in);
		int clientFd = accept(sock, (struct sockaddr*) &clientAddress, &size);
        if (clientFd == -1)
            continue;

        threadArg *arg = (threadArg *) malloc(sizeof(struct threadArg));
		
		arg->clientFd = clientFd;
		arg->clientIp = inet_ntoa(clientAddress.sin_addr);
		arg->clientPort = clientAddress.sin_port;

        printf("open connection from %s:%d\n", arg->clientIp, arg->clientPort);
        
        pthread_mutex_lock(&lock);

		if(totalClients == MAX_CONCURRENT_CLIENTS) {
			printf("Server is busy right now, just seconds\n");
			close(clientFd);
			free(arg);
			continue;
		}
		else {
			pthread_create(&threads[count], NULL, threadFunction, (void*) arg);
			count += 1;
			totalClients += 1;
			if (count == MAX_CONCURRENT_CLIENTS)
				count = 0;
		}

		pthread_mutex_unlock(&lock);
    }
    // Close connection
    close(sock);
    
    return 0;
}

int main(int argc, char *argv[]) {

    int server_port;

    if (argc == 2) { // 1 arguments
        server_port = atoi(argv[1]);
    } else {
        printf("Invalid or less number of arguments provided\n");
        printf("./server <server Port>\n");
        exit(0);
    }

    // Server (Reducer) code
    if (server(server_port) != 0) {
        perror("Server connected failed!");
        return -1;
    }

    return 0;
} 