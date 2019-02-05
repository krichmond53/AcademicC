////////////////////////////////////////////////////////////////////////////////
//
//  File           : a2support.c
//  Description    : This is the support file for assign2.c file.  It includes the 
//  		implementation for the malloc, free, calloc and realloc functions
//
//
//   Author        : Kevin Richmond 
//   Last Modified : 10.14.16 
//

// Include Files
// #include <stdio.h>
// #include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Structures

struct block_meta {
	size_t size;
	struct block_meta *next;
	int free;
	int magic; // For debugging only. TODO: remove this in non-debug mode.
	char data[1];   // ADDED FOR SPLITTING PURPOSES 
};
#define META_SIZE sizeof(struct block_meta)
#define ALIGN8(x) (((((x)-1)>>3)<<3)+8)        // Change x to a mult of 8 for align
void *global_base = NULL;

// Functional Prototypes

/////////////////////////////////////////////////////////////////////////////////
//
//	Function: find_free_block
//	Description: Iterates through a linked list to see if there's a free 
//		memory block which is large enough to hold that data that needs 
//		to be copied into the heap.
//
//	Input: block_meta - Pointer to the pointer of the last block in the linked
//		list
//		size - the size required by this new block
//	Output: the next block in the linked list that that fulfills
//		the needs of the data which is attempting to be copied into the
//		heap
//
struct block_meta *find_free_block(struct block_meta **last, size_t size) {
	struct block_meta *current = global_base;
	while (current && !(current->free && current->size >= size)) {
		*last = current;
		current = current->next;
	}	
	return current;
}

struct block_meta *find_best_fit(struct block_meta **last, size_t size) {
	struct block_meta *current = global_base;
	struct block_meta *best;
	long diff = 9999999999999999;

	while (current) {
		if (current->free && current->size == size){
			return current;
		} 
		if (current->free && current->size > size) {
			if (current->size-size < diff) {
				diff = current->size-size;
			//	*last = current;
				best = current;

			} 
			else {
				*last = current;
				current = current->next;
			}
		} 
		else {
			*last = current;
			current = current->next;
		}
	}	
	if (diff < 999999999999999){
		best->size = size;
		best->magic = 0x33333333;
		return best;
	} else
		return current;
}

///////////////////////////////////////////////////////////////////////////////
//
//	Function: request_speace
//	Description: If there is not space enough already allocated in the linked 
//		list, a request from the OS will have to be made (using sbrk) and 
//		the new block will be added at the end of the linked list.
//
//	Input: block_meta - Pointer to the last block in the linked list
//		size - the size required by this new block 
//	Output: first address of the block which has been added to the end 
//		of the linked list
//
struct block_meta *request_space(struct block_meta* last, size_t size) {
	struct block_meta *block;
	block = sbrk(0);
	void *request = sbrk(size + META_SIZE);
	assert((void*)block == request); // Not thread safe.
	if (request == (void*) -1) {
		return NULL; // sbrk failed.
	}
	if (last) { // NULL on first request.
		last->next = block;
	}
	block->size = size;
	block->next = NULL;
	block->free = 0;
	block->magic = 0x12345678;
	return block;
}

void splitter(struct block_meta *block, size_t s){
	void *ptr = ((char *)block)+s+META_SIZE;
	struct block_meta *newBlock = ptr; 		// Declaration of new structure
		// Must declare as pointer of struct type in order
		// to create a pointer to the beginning of the data portion
	newBlock->size = block->size-s-META_SIZE;
	newBlock->next = block->next;
	newBlock->free = 1;
	newBlock->magic = 0xcccccccc; 			
	block->size = s;
	block->next = newBlock;
}

void merge_blocks(){//struct block_meta *block){//){
	struct block_meta *block = global_base;
	while(block&&block->next){
		if (block->free == 1 && block->next->free == 1){
			block->size = block->size + block->next->size + META_SIZE;
			block->next = block->next->next;
			block->magic = 0x99999999;
		}
		else 
			block = block->next;
	}
}

/////////////////////////////////////////////////////////////////////////
//
//	Function: malloc
//	Description: If the base point is NULL (there is nothing in the linked list)
//		then request space and set the base pointer to our new block.  If it's
//		not NULL, the current linked list is check to see if there is any
//		block sufficient for out new block.  If so, then we put out block
//		there.  If not, additional space is requested and the current block
//		is put into this new space.
//
//	Input: size - the size required by this new block
//	Output: poniter to the address of the region after block_meta

void *malloc(size_t size) {
	struct block_meta *block;
	// TODO: align size?
	size = ALIGN8(size);
	if (size <= 0) {
		return NULL;
	}

	if (!global_base) { 	// First call.
		block = request_space(NULL, size);
		if (!block) {
			return NULL;
		}	
		global_base = block;
	} else {
		struct block_meta *last = global_base;
		block = find_best_fit(&last, size);	//find_free_block(&last, size);
		if (!block) { 	// Failed to find free block.
			block = request_space(last, size);
			if (!block) {
				return NULL;
			}
		} else {	// Found free block
			// TODO: consider splitting block here.
			if ((block->size - size) >= (META_SIZE+8)){
				splitter(block, size);	//Block is big enough, split
			}
			block->free = 0;
//			block->magic = 0x77777777;
		}
	}
	return(block+1);
	//return(block->data); // CHANGED TO ADD BUG >>> return(block+1);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function: get_block_ptr
//	Description: Returns the address of the block_meta struture
//
//	Input: *ptr - pointer that will receive that address of the block_meta 
//		structure
//	Output: address just before the block_meta structure
//

struct block_meta *get_block_ptr(void *ptr) {
	return (struct block_meta*)ptr-1;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function: free
//	Decription: Deallocates the memory assigned by the malloc/calloc function
//
//	Input: *ptr - pointer to a block of memory allocated by malloc/calloc
//	

void free(void *ptr){
	if (!ptr){
		return;
	}

	// TODO: consider merging blocks once splitting blocks is iimplemented.
	struct block_meta* block_ptr = get_block_ptr(ptr);
	assert(block_ptr->free == 0);
	assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
	block_ptr->free = 1;
	block_ptr->magic = 0x55555555;

	//struct block_meta* block =(struct block_meta *) ptr;
	//if (block->free == 1 && block->next->free == 1){
		merge_blocks();          //block);
	//}
}

/////////////////////////////////////////////////////////////////////////
//
//	Function: realloc
//	Description: Readjust the size of a block of memory originally assigned 
//		by either malloc, calloc, or realloc
//
//	Input: *ptr - pointer to the beginning of a memory block
//		size - new size of the block 
//	Output: pointer to the beginning of the memory block
//	

void *realloc(void *ptr, size_t size){
	size = ALIGN8(size);
	if (!ptr) {
	// NULL ptr. realloc should act like malloc.
		return malloc(size);
	}
	struct block_meta* block_ptr = get_block_ptr(ptr);
	if (block_ptr->size >= size) {
		// We have enough space. Could free some once we implement split.
		if ((block_ptr->size - size) >= (META_SIZE+8)){
  			splitter(block_ptr, size);	//Block is big enough, split
		}
		merge_blocks();
		return ptr;
	}

	// Need to really realloc. Malloc new space and free old space.
	// Then copy old data to new space.
	void *new_ptr;
	new_ptr = malloc(size);
	if (!new_ptr) {
		return NULL; // TODO: set errno on failure.
	}	
	memcpy(new_ptr, ptr, block_ptr->size);
	free(ptr);
	return new_ptr;

}

//////////////////////////////////////////////////////////////////////////////
//
//	Function: *calloc
//	Description: Allocates a block of memory and clears every memory address
//
//	Input: nelem - number of elements in the new memory block
//		elsize - size of each element in the new memory block
//	Output: pointer to the new memory block
//

void *calloc(size_t nelem, size_t elsize) {
	size_t size = nelem * elsize; // TODO: check for overflow.
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void report(){ 
	struct block_meta *block = global_base;
	int i =0 , end, leak;

	while(block){ //list constinues till it reaches a null 
		printf("Block %d:\n\tStart: %p  Free: %d  Size: %ld  Next %p\n", i, block, block->free, block->size, block->next); 
		end = (int)block+(block->size)+META_SIZE;
		leak = (int)block->next - end;
		if(block->next)
			printf("\tMagic: 0x%x  End: %p\tUnused memory: %d\n", block->magic, end, leak);
		else
			printf("\tMagic: 0x%x  End: %p\t\n", block->magic, end);

		i++;
		if(block->next) 
			block=block->next; 
		else {
			printf("\n"); 
		 	return;
		}
	} 
}


