#ifndef CMSC257_A1SUPPORT_INCLUDED
#define CMSC257_A1SUPPORT_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File          : cmsc257-f16-assign1-support.h
//  Description   : This is a set of general-purpose utility functions we use
//                  for the 257 assignment #1.
//
//  Author   : Kevin Richmond
//  Created  : 10.02.16

//
// Functional Prototypes

void float_display_array(float fl_array[], int length);
	// This function prints out the array of floating point values

void integer_display_array(int int_array[], int legnth);
	// This function prints out the array of integer values

int float_evens(float fl_array[], int length);
	// Return the number of even numbers in the array (float version)

int integer_evens(int int_array[], int length);
	// Return the number of even numbers in the array (int version)

int countBits(int toCount);
	// Count the number of '1' bits in the binary representation

void most_values(int int_array[], int num_elements, int maxValue);
	// Print out the values with the most occurences in array

unsigned short reverseBits(unsigned short numToReverse);
        // Reverse the binary bit string

void binaryString(char *str, int length, int numToConvert);
        // Print the binary equivalent of the number passed

void integerQuickSort(int int_array[], int lIndex, int rIndex);
        // Sort the integer array using QuickSort algorithm
        

#endif // CMSC257_A1SUPPORT_INCLUDED
