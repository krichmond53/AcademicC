//////////////////////////////////////////////////////////
//
//	Kevin Richmond
//	CMSC257 
//	Project 5 - Sample FTP Client
//	Due Date - Wednesday, Nov 30th
//
//	Description: Client and server programs will be implemented in order
//		to demonstrate the use of network sockets 
//	

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <fcntl.h>
//#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

char buffer[50];
int server, client, halt = 1;

void signal_handler(int val);
int server_operation(void){

//	int server, client;
	int *yes;
	uint32_t inet_len;
	struct sockaddr_in saddr, caddr;
//	char buffer[50];
	FILE *file;
	char nameOfFile[50];

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(2222);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Create server socket
	server = socket(PF_INET, SOCK_STREAM, 0);
	if (server == -1) {
		printf("Error on socket creation [%s]\n", strerror(errno));
		return(-1);
	}
	// Change socket options to reuse server socket immediately
	if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
                perror("setsockopt");
                exit(1);
        }

	// Bind the socket to the address being used
	if (bind(server, (struct sockaddr *)&saddr, sizeof(struct sockaddr)) == -1){
		printf("Error on socket bind [%s]\n", strerror(errno));
		return (-1);
	}	

	// Listen for up to 5 clients requesting access to the server
	if (listen(server, 5) == -1) {
		printf("Error on socket listen [%s]\n", strerror(errno));
		return(-1);
	}

	//  Infinite loop to accept client and process data until ctrl-C
	while(halt) {
		halt = 0;	
	
		// Create a place for the address structure and put the client there
		inet_len = sizeof(caddr);
		if ((client = accept(server, (struct sockaddr *)&caddr, &inet_len)) == -1 ) {
			printf("Error on client accept [%s]\n", strerror(errno));
			close(server);
			return(-1);
		}
		printf("Server new client connection [%s/%d]\n", inet_ntoa(caddr.sin_addr), caddr.sin_port);
		
		// Receive the first line, which should be the file name
		if (read(client, &buffer, sizeof(buffer)) != sizeof(buffer)) {
			printf("Error reading network data [%s]\n", strerror(errno));
			close(server);
			return(-1);
		}
 
		// Report name of file requested
		strcpy(nameOfFile, buffer);
		printf("Client requests file [%s]\n", nameOfFile);
		
		// If buffer is a file name, open that file
		file = fopen(buffer, "r");
		
		// Send the client the file name to verify it's available
		if (write(client, &buffer, sizeof(buffer)) != sizeof(buffer)) {
			printf("Error writing network data [%s}\n", strerror(errno));
			close(server);
			return(-1);
		}
		// printf("Sent a value of [%s]\n", buffer);
	
		// Put data into the buffer and send 50 bytes at a time
		while (fgets(buffer, sizeof(buffer), file) ){
			halt = 1;
			// sleep(1);
			 
			if (write(client, &buffer, sizeof(buffer)) != sizeof(buffer)) {
				printf("*Error writing network data [%s}\n", strerror(errno));
				close(server);
				return(-1);
			}
			if (!halt){
				strcpy(buffer, "cmsc257");
				if (write(client, &buffer, sizeof(buffer)) != sizeof(buffer)){
					printf("Client socket closed.  [%s]\n", strerror(errno));
					close(server);
					return(-1);		
				}

			}
			
			// printf("Sent a value of [%s]\n", buffer);
		}
		
		// When the file is done, send cmsc257 to signal client termination
		strcpy(buffer, "cmsc257");
		if (write(client, &buffer, sizeof(buffer)) != sizeof(buffer)){
			printf("***Error writing network data [%s]\n", strerror(errno));
			close(server);
			return(-1);		
		}
		// printf("Sent a value of [%s]\n", buffer);
		printf("File [%s] sent successfully.\n\n", nameOfFile);		

		close(client);		// Close the socket
	}
	return (0);
}

int main(int argc, char **argv){

	struct sigaction new_action, old_action;	// Setup the sig actions
	new_action.sa_handler = signal_handler;
	new_action.sa_flags = SA_NODEFER | SA_ONSTACK;
	sigaction(SIGINT, &new_action, &old_action);
		

	server_operation();
	printf("****IN MAIN****\n");
	return(0);
}

void signal_handler(int val){  // When ^C is pressed, gracefully exit
		
	if (val == SIGINT) {
		//printf("Interrupted - Halt is %d\n", halt);
		if (halt){
			printf("\nCurrent transfer cancelled.\n");
			halt = 0;
	 		strcpy(buffer, "break_cmsc257");
			if (write(client, &buffer, sizeof(buffer)) != sizeof(buffer)){
				printf("Closed client socket [%s]\n", strerror(errno));
				close(server);
			} else {
		 		sleep(1);
				printf("Client socket closed.  Shutting down server. [%s]\n", strerror(errno));
				exit(0);
			}	
		} else {
			printf("\nNo transfer in progress.\nShutting down server. [%s]\n", strerror(errno));
			exit(0);
		}
	} 
	return;
}









