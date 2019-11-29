#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

void error(char *msg){	//exec on syscall error, print msg to stderr
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
    int sockfd;
    int newsockfd;
    int portno;
    int clilen;
    int n;
    char buffer[256];

    struct sockaddr_in serv_addr;

    time_t Ctime;
    time_t Ptime = 0;
    time_t interval = 2;

    if(argc < 3){
	    printf("usage %s hostname port", argv[0]);
	    exit(1);
    }

    portno = atoi(argv[2]); //Set portno from input argv

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
	    error("ERROR opening socket");
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET; //set address family i.e AF_INET or AF_UNIX for internet or local
    serv_addr.sin_port = htons(portno); //htons converts portno to Big Endian format
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);	//Set IP of server to connect

    if( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
	error("ERROR connecting to server");
    }

    printf("Enter message: ");
   
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n=write(sockfd, buffer, strlen(buffer));
    if(n < 0){
        error("ERROR writing to socket");
    }
    while(1){
        Ctime = time(NULL);
        if(Ctime - Ptime > interval){
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            if(n < 0){
                error("ERROR reading from socket");
            }
            printf("%s\n", buffer);
            Ptime = Ctime;
        }
    }

    close(sockfd);
    return 0;
}
