//#ifndef A2SUPPORT_INCLUDED
//#define A2SUPPORT_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File          : assign2-support.h
//  Description   : This is a set of declarations used for the functions required
//                  in assignment 2.
//
//  Author   : Kevin Richmond
//  Created  : 10.14.16

//
// Functional Prototypes

struct block_meta *find_free_block(struct block_meta **last, size_t size);
	// Find the first free block in the linked list
	
struct block_meta *request_space(struct block_meta* last, size_t size);
	// Request free space beyond current linked list

void splitter(struct block_meta *block, size_t s);
	// If there is enough space, create a new block

void merge_block();
	//  If there are 2 contiguious free blocks, merge them

void *malloc(size_t size);
	// Allocates new block of memory
	
struct block_meta *get_block_ptr(void *ptr);
	// Returns the address of the block_meta structure
	
void free(void *ptr);
	// Dealllocates memory assignments

void *realloc(void *ptr, size_t size);
	// Readjust the size of a block of memory already assigned

void *calloc(size_t nelem, size_t elsize);
	// Allocates new block of memory and clears it        

void report();
	// Prints out each node and its information
	
//#endif A2SUPPORT_INCLUDED
