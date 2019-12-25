#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define num_of_cli 5

void error(char *msg){	//exec on syscall error, print msg to stderr
	perror(msg);
	exit(1);
}

void *image_transmit_thread(void *arg){
	int newSocket = *((int *)arg);
	int bytesWritten = 0;

	char viewConMes[] = "Message from server: Viewer connected";
	
	bytesWritten = write(newSocket, &viewConMes, sizeof(viewConMes));
	if(bytesWritten < 0){
		printf("Error writing to socket");
	}

	close(newSocket);
	pthread_exit(NULL);
}

void *controller_input_thread(void *arg){
	int new_socket = *((int *)arg);
	int bytesWritten = 0;

	char contConMes[] = "Message from server: Controller connected";
	
	bytesWritten = write(new_socket, &contConMes, sizeof(contConMes));
	if(bytesWritten < 0){
		printf("Error writing to socket");
	}

	close(new_socket);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int server_socket;
    int client_socket;
    int server_port_nr;
    int clientAddressLength;

    char buffer[10];
    char viewConnect[10] = "screenview";
	char contConnect[10] = "controller";
	char CliLimMes[] = "No more clients accepted";
    
	struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    if(argc < 2){
	    printf("ERROR no port provided");
	    exit(1);
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
	    error("ERROR opening socket");
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_port_nr = atoi(argv[1]); //Sets portno from input argv - prob. change to static in final imp.

    server_address.sin_family = AF_INET; //set address family i.e AF_INET or AF_UNIX for internet or local
    server_address.sin_port = htons(server_port_nr); //htons converts portno to Big Endian format
    server_address.sin_addr.s_addr = INADDR_ANY;	//Sets server IP to whatever given from dhcp

    if( bind(server_socket, (struct sockaddr *) &server_address,
	sizeof(server_address)) < 0){
		error("ERROR on socket binding");
    }

	//Vi er kommet hertil - listen() skal måske ind i while(1) løkken 

    if( listen(server_socket, 5) < 0 ){
		error("Error on listen function");
	}	//Listen on the socket for connections, 5 the number of waiting connections
   
	pthread_t thread[num_of_cli];	//Create threads

	int i = 0;
	while(1){
		clientAddressLength = sizeof(client_address);

		//accept function blocks process until a connection is made.
		//e.i process waits for connections.
		client_socket = accept(server_socket, (struct sockaddr *)
		&client_address, &clientAddressLength);
    
		bzero(buffer, 10); //initialize/zero-out buffer 
    
		//reads from socket + error check
		if( read(client_socket, buffer, 10) < 0 ){
			error("ERROR reading from socket");
		} else{
			if(strcmp(&buffer, &viewConnect) == 0){
				// Check for thread creation
				if( pthread_create(&thread[i], NULL, image_transmit_thread,
				&client_socket) != 0 ){
					printf("Failed to create image transmit thread");
				} else{
					printf("image thread created");
					i++;
				}
			} 
			if(strcmp(&buffer, &contConnect) == 0){
				// Check for thread creation
				if( pthread_create(&thread[i], NULL, controller_input_thread,
				&client_socket) != 0 ){
					printf("Failed to create controller thread");
				} else{
					printf("controller thread created");
					i++;
				}
			}
			if(strcmp(buffer, viewConnect) != 0 && strcmp(buffer, contConnect)
			!= 0){
				printf("No connections matching image viewer or controller");
			}
		}
		if(i>num_of_cli){
			write(server_socket, &CliLimMes, sizeof(CliLimMes));
			i = 0;
			while(i < num_of_cli){
				pthread_join(thread[i], NULL);
				i++;
			}
		}
	}
	close(server_socket);
    return 0;
}
