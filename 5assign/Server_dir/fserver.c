//////////////////////////////////////////////////////////
//
//	Kevin Richmond
//	CMSC257 
//	Project 5 - Sample FTP Client
//	Due Date - Wednesday, Nov 30th
//
//	Description: Client and server programs will be implemented in order
//		to demonstrate the use of network sockets 
//	** THIS IS THE FORKED VERSION OF THE SERVER **

#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

char buffer[50], kidbuffer[50];
int server, client, kid, halt = 1;

void signal_handler(int val);
int server_operation(void){

	int *yes, pid;
	uint32_t inet_len;
	struct sockaddr_in saddr, caddr;
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

	// Listen for up to 1 clients requesting access to the server
	if (listen(server, 5) == -1) {
		printf("Error on socket listen [%s]\n", strerror(errno));
		return(-1);
	}
	

	//  Infinite loop to accept client and process data until ctrl-C
	while(halt) {
		// halt = 0;	
		
		// Create a place for the address structure and put the client there
		inet_len = sizeof(caddr);
		if ((client = accept(server, (struct sockaddr *)&caddr, &inet_len)) == -1 ) {
			printf("Error on client accept [%s]\n", strerror(errno));
			close(server);
			return(-1);
		}
		printf("Server new client connection [%s/%d]\n", inet_ntoa(caddr.sin_addr), caddr.sin_port);
	
	

	// ******Start creating child processes for up to 5 server ports
	pid = fork();
	if (pid < 0) {
			perror("ERROR on fork");
			exit(1);
	}

	if (pid == 0) {
		halt = 0;
		kid = client;
		char kidbuffer[50]; 
	
		// Receive the first line, which should be the file name
		if (read(kid, &kidbuffer, sizeof(kidbuffer)) != sizeof(kidbuffer)) {
			printf("Error reading network data [%s]\n", strerror(errno));
			close(server);
			return(-1);
		}
 
		// Report name of file requested
		strcpy(nameOfFile, kidbuffer);
		printf("Client requests file [%s]\n", nameOfFile);
		
		// If buffer is a file name, open that file
		file = fopen(kidbuffer, "r");
		
		// Send the client the file name to verify it's available
		if (write(kid, &kidbuffer, sizeof(kidbuffer)) != sizeof(kidbuffer)) {
			printf("Error writing network data [%s}\n", strerror(errno));
			close(server);
			return(-1);
		}
	
		// Put data into the buffer and send 50 bytes at a time
		while (fgets(kidbuffer, sizeof(kidbuffer), file) ){
			halt = 1;
			//sleep(2); 	// DELAY FOR TESTING
			 
			if (write(kid, &kidbuffer, sizeof(kidbuffer)) != sizeof(kidbuffer)) {
				printf("*Error writing network data [%s}\n", strerror(errno));
				close(server);
				return(-1);
			}
			if (!halt){
				strcpy(kidbuffer, "cmsc257");
				if (write(kid, &kidbuffer, sizeof(kidbuffer)) != sizeof(kidbuffer)){
					printf("Client socket closed.  [%s]\n", strerror(errno));
					close(server);
					return(-1);		
				}
			}
		}
		
		// When the file is done, send cmsc257 to signal client termination
		strcpy(kidbuffer, "cmsc257");
		if (write(kid, &kidbuffer, sizeof(kidbuffer)) != sizeof(kidbuffer)){
			printf("***Error writing network data [%s]\n", strerror(errno));
			close(server);
			return(-1);		
		}
		// printf("Sent a value of [%s]\n", buffer);
		printf("File [%s] sent successfully.\n\n", nameOfFile);		

	
	// Final part of pid == 0 and the else statement
		//printf("Halt = %d\n", halt);
		exit(0);
	} else {
		close(client);
	}



		halt = 1;	
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
		if (halt){
			halt = 0;
	 		strcpy(kidbuffer, "break_cmsc257");
			// write(client, &buffer, sizeof(buffer));
			if (write(kid, &kidbuffer, sizeof(kidbuffer)) != sizeof(kidbuffer)){
				printf("Closed client socket\n");
				close(server);
			} 
			else {
				sleep(1);
				printf("\n**Client socket closed.\n");
				exit(0);
			}	
		} else {
			printf("\nNo transfer in progress.\n"); 
			exit(0);
		}
		printf("Shutting down server.\n");
		exit(0);
	} 
	return;
}









