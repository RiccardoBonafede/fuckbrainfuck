#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DOUBLING_CONST 2 

void _mem_allocation_error(const char *functionName){
		fprintf(stderr, "<CRITICAL: %s>\n: Error while allocating new memory\n", functionName);
		exit(EXIT_FAILURE);
}


/**
 * Insert n bytes to an array. The array has to be allocated with a previus malloc (or similar function) call
 *
 * \param void **arrayPtr Pointer to the array pointer
 * \param void *bytes Pointer to the data to insert
 * \param size_t bytes_to_write Size of the data to insert
 * \param size_t ndata Number of bytes in the array
 * \param size_t array_size Size of the prevus allocated array
 * \return Size of the new array
*/

size_t insertIntoArray(void ** arrayPtr, void * bytes, size_t bytes_to_write,size_t ndata ,size_t array_size){
	int new_array_size = array_size;

	while(new_array_size <= bytes_to_write + ndata){
		new_array_size *= DOUBLING_CONST;

		*arrayPtr = realloc (*arrayPtr, new_array_size);

		if(*arrayPtr == NULL)
			_mem_allocation_error(__func__);
	}

	memcpy((*arrayPtr) + ndata, bytes, bytes_to_write);
	return new_array_size;

}