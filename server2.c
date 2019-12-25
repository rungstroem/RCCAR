#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

void error(char *msg){	//exec on syscall error, print msg to stderr
	perror(msg);
	exit(1);
}

void *img_transmit_thread(void *arg){
	int newSocket = *((int *)arg);
	
	time_t Ctime;
	time_t Ptime = 0;
	time_t interval = 5;
	int BytesWritten = 0;
	char viewConMes[] = "Viewer connected";
	while(1){
		Ctime = time(NULL);
		if(Ctime-Ptime > interval){
			BytesWritten = write(newSocket, &viewConMes, sizeof(viewConMes));
			if(BytesWritten < 0){
				printf("Error writing to socket");
				error("Error writing to socket");
			}
			Ptime = Ctime;
		}
	}
	close(newSocket);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int sockfd;
    int newsockfd;
    int portno;
    int clilen;
    int n;

    char buffer[10];
    char viewConnect[10] = "screenview";
	char contConnect[10] = "controller";
	char CliLimMes[] = "No more clients accepted";
    
	struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;

	pthread_t thread1;
    
    if(argc < 2){
	    printf("ERROR no port provided");
	    exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
	    error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]); //Sets portno from input argv - prob. change to static in final imp.

    serv_addr.sin_family = AF_INET; //set address family i.e AF_INET or AF_UNIX for internet or local
    serv_addr.sin_port = htons(portno); //htons converts portno to Big Endian format
    serv_addr.sin_addr.s_addr = INADDR_ANY;	//Sets server IP to whatever given from dhcp

    if( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
	error("ERROR on socket binding");
    }

    listen(sockfd, 5);  //Listen on the socket for connections, 5 the number of waiting connections
    clilen = sizeof(cli_addr);

	//Loops around to wait for more connections
	int i = 0;
	while(1){
		//accept function blocks process until a connection is made.
		//e.i process waits for connections.
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    
		bzero(buffer, 10); //initialize/zero-out buffer 
    
		//reads from socket
		n = read(newsockfd, buffer, 10);
		if(n < 0){
			error("ERROR reading from socket");
		}
    
		printf("From client: %s", buffer);
    
		if(strcmp(buffer, viewConnect) > -1){
			pthread_create(&thread1, NULL, img_transmit_thread, &newsockfd);
		}
		if(strcmp(buffer, contConnect) > -1){
			//pthread_create();
		}
		i++;
	}
	write(sockfd, &CliLimMes, sizeof(CliLimMes));
	close(sockfd);
    return 0;
}
