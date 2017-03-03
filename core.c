#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "core.h"

struct memory_8b *load_file_8b(char *filename, struct memory_8b *memory){
	FILE *progFd;
	//struct memory_8b *memory;
	size_t allocatedSize;
	char c=0;
	int readedChars=0;

	memory->programCode = malloc(ARRAY_START_LENGHT * sizeof(char));
	if(memory->programCode == NULL) _mem_allocation_error(__func__);

	allocatedSize = ARRAY_START_LENGHT * sizeof(char);

	/*Open file and chek if all is ok*/
	progFd = fopen(filename, "r");
	if(progFd==NULL){
		fprintf(stderr, "<CRITICAL:%s>\n Error: Cannot open file %s\n", __func__ , filename);
		exit(EXIT_FAILURE);
	};

	//Legge il codice dal file
	while(!feof(progFd)){
		fscanf(progFd, "%c", &c);
		
		if(strchr(VALID_CHARACTERS, c) && c!=0x00){	//ignora tutto il resto
			allocatedSize = insertIntoArray ((void **) &memory->programCode, &c, sizeof(c), readedChars * sizeof(c), allocatedSize);
			readedChars++;	
		}
		c=0;

	}
	memory->programSize = readedChars - 1;
	fclose(progFd);
	return memory;

}