////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmsc257-f16-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMSC257.  See the related
//                   assignment page for details.
//
//   Author        : Kevin Richmond 
//   Last Modified : 10.04.16 
//

// Include Files
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Project Includes
// #include <cmsc257-f16-assign1-support.h>
#include "cmsc257-f16-assign1-support.h"
//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMSC257 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main(int argc, char *argv[]) {

	// Local variables
	// NOTE: this is where you will want to add some new variables
        int sizeOfArray = 20, mask, i, temp;	
	float f_array[sizeOfArray];
	int m_array[sizeOfArray]; 
	char binary[17] = {0};
	char revBinary[17] = {0};

	// Step #1 - read in the float numbers to process
	for (i=0; i<20; i++) {
		scanf("%f", &f_array[i]);
	}

    	// Step #2 - convert the floating point values to integers
	for (i=0; i<20; i++) {
		temp  = (int) f_array[i];       // Convert float to int by casting
		mask = temp>>31;		// Push sign bit to the right for mask
		temp = (temp+mask)^mask;        // Add mask & temp then XOR for + num
		m_array[i] = temp&15;		// XOR by 16-1 (or 15)  for mod 16
	}

	// Step #3 - print out the integer and floating point array
	printf("Floats\n");
	float_display_array(f_array,20);

	printf("\n\nIntegers\n");	
	integer_display_array(m_array, 20);

	// Step #4 - print out the number of '1's in each integer
	printf("\n\nNumber of bits per int (Format: int/bit)\n");
	for (i=0; i<20; i++){
		printf("%d/%d\t", m_array[i], countBits(m_array[i]));
	}	

	// Print a binary string representation of each integer
	printf("\n\nBinary representation of each integer:\n");
	for (i=0; i<20; i++) {
		binaryString(binary, 15, m_array[i]);
	}
	

	// Step #5 - Sort the integer array using QuickSort and print the sorted array
	integerQuickSort(m_array, 0, 19);


	printf("\n\nThe QuickSorted array is:\n");
	for (i=0; i<20; i++){
		printf(" %d", m_array[i]);
	}

	// Step #6 - figure out number of evens in each array (float and sorted integer arrays)
	printf("\n\n%d even integers.\n", integer_evens(m_array, 20));

	printf("\n%d even floats.\n", float_evens(f_array, 20));


	// Step #7 - print the most frequent values from the sorted integer array
	printf("\nThe integer(s) that occur most is/are: ");
	most_values(m_array, 20, 15);

	// Step #8 - print the integers corresponding to the reverse bit string from integer array
	printf("\nReverse bits of inegers. (Format rev/int).\n");
	for (i=0; i<20; i++){
		printf("%d/%d\t", reverseBits(m_array[i]), m_array[i]); 
	}
 	
	printf("\n\nReverse bits of integers (in binary form).\n");
	
	for(i=0; i<20; i++){
		binaryString(revBinary, 15, reverseBits(m_array[i]));	
	}
	printf("\n\n");
	// Return successfully
	return(0);
}
