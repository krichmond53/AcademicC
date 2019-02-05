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
//#include <signal.h>
#include <errno.h>

int client_operation(char *filename){
	
	int socket_fd; //, openFile;//, yes;
	//uint32_t value;
	struct sockaddr_in caddr;
	char *ip = "127.0.0.1";
	char buffer[50];
	FILE *fp;
	
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(2222);
	if (inet_aton(ip, &caddr.sin_addr) == 0 ){
		return(-1);
	}

	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
/*	// Adding socket options to limit wait times
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("setsockopt");
		exit(1);
	}
*/
	if (socket_fd == -1) {
		printf("Error on socket creation [%s}\n", strerror(errno));
		return(-1);
	}

	if (connect(socket_fd, (const struct sockaddr *)&caddr, sizeof(struct sockaddr)) == -1 ){
		printf("Error on socket connect [%s]\n", strerror(errno));
		return(-1);
	}

	strcpy(buffer, filename);
	if (write(socket_fd, &buffer, sizeof(buffer)) != sizeof(buffer)){
		printf("Error writing network data [%s]\n", strerror(errno));
		return(-1);
	}
	// printf("Sent a value of [%s] of size [%lu]\n", buffer, sizeof(buffer));  

	if (read(socket_fd, &buffer, sizeof(buffer)) != sizeof(buffer)){
		printf("Error reading network data [%s]\n", strerror(errno));
		return(-1);
	}
	// printf("Received a value of [%s]\n", buffer);
	
	// Open file to receive data, using filename
	fp = fopen(buffer, "w");
	if (fp == NULL) {
		printf("Error opening file!\n");
		exit(1);
	}
	//fprintf(fp, "%s", buffer);

	//FILE *fromFile = buffer;
	//while(fgets(buffer, sizeof(buffer), fromFile)) {
	while(1){
		if (read(socket_fd, &buffer, sizeof(buffer)) != sizeof(buffer)){
			printf("Error reading network data [%s]\n", strerror(errno));
			return(-1);
		} 
		if (strcmp(buffer,"cmsc257") == 0){
			printf("Transfer of [%s] complete\n", filename);
			break;
		}  
		if (strcmp(buffer,"break_cmsc257") == 0){
			printf("Transfer interrupted.  [%s] incomplete.\n", filename);
			break;
		} 
		fprintf(fp, "%s", buffer);
		// printf("Received a value of [%s]\n", buffer);
		
	}

	fclose(fp);		

	// Loop - Put buffer data into file

	close(socket_fd);		// Close the socket
	return(0);
}

int main(int argc, char **argv){
	char buffer[50];

	// Accepts the first command line argument as the file to fetch
	if (argc >=2){
		strcpy(buffer, argv[1]);
		printf("\nFile to fetch: %s\n", buffer);
	}

	client_operation(buffer);
	return(0);
}
