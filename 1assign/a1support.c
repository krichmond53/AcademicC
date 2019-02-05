////////////////////////////////////////////////////////////////////////////////
//
//  File           : a1support.c
//  Description    : This is the support file for cmsc257-f16-assign1.c file.  It 
//  		includes the code for the functions defined in 
//  		cmsc257-f16-assign1-support.h.  See the related assignment page 
//  		for details.
//
//   Author        : Kevin Richmond 
//   Last Modified : 10.05.16 
//

// Include Files
#include <stdio.h>
#include <stdlib.h>

// Functional Prototypes

////////////////////////////////////////////////////////////////////////////////
//
//	Function: float_display_array
//	Description: Prints out the array of floating integers to the screen
//
//	Input: fl_array[] - array of values in floating point form
//		length - size of the array
//
void float_display_array(float fl_array[], int length){
	int i;
	for (i=0; i<length; i++) {  // Go through the array printing each float
		printf("%2.2f\t", fl_array[i]);
	}

}

///////////////////////////////////////////////////////////////////////////////
//
//	Function: integer_display_array
//	Description: Prints out the array of integer values
//
//	Input: int_array[] - array of values in integer form
//		length - size of the array
//
void integer_display_array(int int_array[], int length){
	int i;	
	for (i=0; i<length; i++) {  // Iterate through array printing each float
 		printf("%2d\t", int_array[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////
//
//	Function: fload_evens
//	Description: Return the number of even numbers in the array (float version)
//
//	Input: fl_array[] - array of values in floating point form
//		length - size of the array
//
int float_evens(float fl_array[], int length){
	int temp, floatIsEven = 0, i;	// floatIsEven set to 0 for counter
	for (i=0; i<length; i++){ 	// Iterate through size of array
		temp = (int)fl_array[i];// Cast float to int
		if ((temp&1) == 0)	// If the LSB of temp & 1 is 0
			floatIsEven++;	// Increase the number of evens
	}
	return floatIsEven;
}
///////////////////////////////////////////////////////////////////////////////
//
//	Function: integer_evens
//	Description: Return the number of even numbers in the array (int version)
//
//	Input: int_array[] - array of values in integer form
//		length - size of the array
//
//	Output: intIsEven - Count of all the even integers in the array
//
int integer_evens(int int_array[], int length){
	int intIsEven = 0, i;		// intIsEven set to 0 for counter
	for (i=0; i<length; i++){	// Iterate through size of array
		if((int_array[i]&1) == 0)// If LSB of value & 1 is 0
	 		intIsEven++;	// Increment number of evens
	}
	return intIsEven;
}

///////////////////////////////////////////////////////////////////////////////
//
//	Function: countBits
//	Description: Returns the number of '1' bits in the binary representation
//
//	Input: toCount - an intger whose 1 bits are to be counted
//	
//	Output: numOf1s - the number of times 1 appears in toCounts' binary string
//
int countBits(int toCount){
	int numOf1s = 0;		// numOf1s is counter
	while(toCount){			// While there is a number to count
		if ((toCount&1) ==1)	// If LSB &1 is 1 (Bit is 1)
			numOf1s++;	// Increment count
		toCount>>=1;		// Move to the next MSB
	}
	return numOf1s;
}

//////////////////////////////////////////////////////////////////////////////////
//
//	Function: most_values
//	Description: Print out the values with the most occurences in the array
//
//	Input: int_array[] - an array of values in integer form
//		num_elements - the number of elements in the array
//		maxValue - the highest possible value integer in the array
//
//	Output: returns 0 if successful
//
int most_values(int int_array[], int num_elements, int maxValue){
	int i, maxCount = 0;
	int numCounted[16] = {0};  		// Initialize array to place counters
	for (i=0; i<num_elements; i++){		// Loop to count from int_array
		int value = int_array[i];	// Whatever value is in int_array
		numCounted[value]++;		// Counter increases for that value
		if (numCounted[value] > maxCount) // If that counter is bigger than
			maxCount = numCounted[value]; // max, then replace max
	}
	for (i=0; i<16; i++){			// Loop to see which values = max
		if (numCounted[i] == maxCount)	// If values = max, print it
			printf("%d ", i);
	}  
	printf("Occuring %d times.\n", maxCount);
	return(0);
}

//////////////////////////////////////////////////////////////////////////////
//
//	Function: reverseBits
//	Description: Reverse the binary bit string
//
//	Input: numToReverse - value whose bits will be reversed
//	Output: revInt - the reverse bit value of the input
//
unsigned short reverseBits(unsigned short numToReverse){
	int i, revInt=0, temp;
	int numBits = 16;
	for (i=0; i<numBits; i++){
		temp = numToReverse&(1<<i);	
		if(temp)
			revInt |= (1<<((numBits-1)-i));
	}
	return revInt;

}

//////////////////////////////////////////////////////////////////////////////////
//
//	Function: binaryString
//	Description: Print the binary equivalent of the number passed
//
//	Input: *str - pointer to an array of chars to store the binary string
//		length - # of bits - 1 in the value to be converted
//		convert - value to be converted to binary string
//
void binaryString(char *str, int length, unsigned short convert){
	int i, mask = 0x8000;	// 16 bit mask of 1000000000000000
	for(i=0; i<=length; i++){ 	// Iterate through # of bits (length)
		if((convert & mask) == 0) // If mask and bit is 0, store 0 in string
			str[i] = '0';
		else  
			str[i] = '1';     // Otherwise store 1 in string.
		mask >>= 1;  		  // Shift mask to check against next MSB
	} 
	printf("%s\t", str);		// Print the string of binary numbers 
}

///////////////////////////////////////////////////////////////////////////////////
//
//	Function: integerQuickSort
//	Description: Print array of integers which have been Quicksorted w/ bitwise swaps
//
//	Input: int_array[] - array of values in integer form
//		low - minimum index in the array
//		high - maximum index in the array
//
void integerQuickSort(int int_array[], int low, int high){
	int pivot, j, i;
	if(low<high) {
		pivot=low;
		i= low;
		j=high;

		while(i<=j){		// If less then pivot, next i index
			while(int_array[i]<int_array[pivot])
				i++;	// If greater than pivot, next j index
			while(int_array[j]>int_array[pivot])
				j--;
			if(i==j) {	// Move to the next i/j index
				i++; j--;
			}
			if(i<j){  // Bitwise swap with XOR, move to next i/j index
				int_array[i] = int_array[j]^int_array[i];
				int_array[j] = int_array[j]^int_array[i];
				int_array[i] = int_array[j]^int_array[i];
				i++; j--;
			}
		}	
		if(low<j){		// Recursive call to sort on left half
			integerQuickSort(int_array, low, j);
		}
		if(high>i){		// Recursive call to sort on right half
			integerQuickSort(int_array, i, high);
		}
	}
}

