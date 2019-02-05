/////////////////////////////////////////////////////////////////////////////////////
//
//
//	Kevin Richmond
//	CMSC257 - Computer Systems
//	Assignment 4
//	Sample Unix Shell
//	Last Update: 11/11/16
//
//	Description:  Unit shell will perform the basic 4 function of exit, pid,
//		 ppid, and cd. All other executables will be performed via fork 
//		 and execvp in the shell.  CTRL-C will not exit out of the shell.
//		 The exit command must be used.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void suicide_signal(void) {
	raise (SIGKILL);
	return; 			// Not gonna happen
}

void signal_handler(int val){
	if (val == SIGINT) {
		printf(" BLOCKED!!  Type 'exit' to quit.\n");
	} 
	return;
}

int main(int argc, char **argv){
	char buffer[50], dumb[50]; 
	char *prompt = "257sh";
	char *word, *delimiter = " ";
	int kidCount = 0, i, execVal = 0;
	pid_t pid;

	struct sigaction new_action, old_action;	// Setup the sig actions
	new_action.sa_handler = signal_handler;
	new_action.sa_flags = SA_NODEFER | SA_ONSTACK;
	sigaction(SIGINT, &new_action, &old_action);
	
	// Allow user to set prompt with -p.  Otherwise default prompt is 257sh>
	if ( argc == 3 ) {
		if (strcmp(argv[1], "-p") == 0) {
			prompt = argv[2];	
		}
	}

	while(1){
		printf("\n%s> ", prompt);
		fgets(buffer, 50, stdin);		// Read multiple strings
		buffer[strcspn(buffer, "\n")] = 0;	// Eliminate the ending \n
		word = strtok(buffer, delimiter);
	
		// Loop to prevent segfault w/ null value or \n only
		while (strlen(buffer) == 1 || word == NULL){	 
			printf("\n%s> ", prompt);
			fgets(buffer, 50, stdin);		
			buffer[strcspn(buffer, "\n")] = 0;	
			word = strtok(buffer, delimiter);
		}
		
		// Ifs for 4 built in commands
		if (strcmp(word,"exit") == 0) {
			suicide_signal();	
		} 
		else if (strcmp(word,"pid") == 0) {
			printf("Current process id: %d\n", getpid());
		} 
		else if (strcmp(word,"ppid") == 0) {
			printf("Parent process id: %d\n", getppid());
		 
		}
		else if (strcmp(word,"cd") == 0) {
			word = strtok(NULL, delimiter);
			if (word == NULL) {		// Show current working dir
				printf("%s\n", getcwd(dumb, sizeof(dumb)));
			} else {			// Change current working dir
				chdir(word);
			}
		}	
		else {		// Deal with all other executable programs
			kidCount++;
			pid = fork();			// Create fork
			if(pid < 0) {			// Fork failed
				printf("Fork failed.  Returned %d\n", pid);	
				exit(1);
			}
			if(pid > 0) {			// Returned parent value
				//printf("Parent: New child is %d\n", pid);
			}
			else {				// Child operation
				char *args[20];		// needed for execvp arg 2
				args[0] = word;		// needed for execvp arg 1
				
				for(i=1; i<20; i++){	// Put args in array
					word = strtok(NULL, " ");
					args[i] = word;
				} 
				
				execVal = execvp((const char*)args[0], args);
				word = strtok(NULL, " ");
		/*		if (execVal < 0){
					printf("Invalid file. Exit status %d\n", execVal);
					exit(1);
				} else { 
			//	printf("Child process %d complete. Exit status %d\n", pid, execVal);
		*/		exit(0);
			 	// }	
			}
		}
		
		int status;
		if (kidCount > 0) {			// For each child, WAIT
			for (i=0; i<kidCount; i++){
				waitpid(pid, &status, 0);    //NULL);
				printf("Child process %d complete. Exit status %d\n", pid, execVal);
				kidCount--;
			}
		}

	}
	return(0);
}
