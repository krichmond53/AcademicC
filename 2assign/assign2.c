////////////////////////////////////////////////////////////////////////////////
//
//  File           : assign2.c
//  Description    : This is the the driver file for assignment to that utilizes
//  		dynamic memory allocation
//
//   Author        : Kevin Richmond 
//   Last Modified : 10.20.16 
//

// Include Files
#include <stdio.h>
//#include <stdlib.h>

// Project Includes
#include "assign2-support.h"


// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMSC257 assignment #2
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main(int argc, char *argv[]) {
	char size = sizeof(char);
	char *ptr[20];
	char *heap_b, *heap_t = sbrk(0);

	printf("\nTop of heap: %p\n", heap_t);
	ptr[0] = malloc(size*80);
	ptr[1] = malloc(size*160);

	ptr[2] = malloc(size*240); 	// malloc(size*32);
	ptr[3] = malloc(size*304);	// malloc(size*32);

	ptr[4] = malloc(size*560);
	ptr[5] = malloc(size*56);
	
	printf("\t>>>>>Malloc 80, 160, 240, 304, 560, 56<<<<<\n");
	report();	
	free(ptr[1]);
	free(ptr[3]);
	free(ptr[5]);
	
	ptr[6] = malloc(size*40);
	ptr[7] = malloc(size*152);
	ptr[8] = malloc(size*248);
	printf("\t>>>>>Free blocks 1, 3, and 5. Malloc 40, 152, 248<<<<<\n\n");
	report();
	heap_b = sbrk(0);
	printf("New top of the heap: %p\n", heap_b);
	printf("Total heap size: %ld bytes\n", heap_b-heap_t);
	return 0;
}
