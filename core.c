#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "utils.h"
#include "core.h"

//unsigned int parseBrackets(struct brackets **bracketsArray, char *programCode, size_t programSize, size_t firstAllocationSize);
u32 get_current_stack_cell(struct memory *program);
void increment_current_stack_cell(struct memory *program);
void decrement_current_stack_cell(struct memory *program);
void assign_to_current_stack_cell(struct memory *program, u32 value);

#define INC_CURRENT_STACK_CELL(pstruct) increment_current_stack_cell(pstruct)
#define DEC_CURRENT_STACK_CELL(pstruct) decrement_current_stack_cell(pstruct)
#define GET_CURRENT_STACK_CELL(pstruct) get_current_stack_cell(pstruct)
#define ASSIGN_TO_CURRENT_STACK_CELL(pstruct, value) assign_to_current_stack_cell(pstruct, value)

#define GET_CELL_SIZE(dim) (1 << dim)
#define GET_STACK_OFFSET(pstruct) pstruct->accumulator * GET_CELL_SIZE(pstruct->type)
#define GET_CURRENT_STACK_PTR(pstruct) pstruct->programStack + GET_STACK_OFFSET(pstruct)

/* Load the file code and stores in the program structure
 *
 * \arg filename the name of the file
 * \arg the memory struct that has to be allocated before
 */
struct memory *load_file(char *filename, struct memory *memory){
	FILE *progFd;
	size_t allocatedSize;
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
		char c=0;
		fscanf(progFd, "%c", &c);
		
		if(strchr(VALID_CHARACTERS, c) && c!=0x00){	//ignora tutto il resto
			allocatedSize = insertIntoArray ((void **) &memory->programCode, &c, sizeof(c), readedChars * sizeof(c), allocatedSize);
			readedChars++;	
		}

	}
	memory->programSize = readedChars - 1;
	fclose(progFd);
	return memory;

}

struct memory *prepare_memory(enum programType ttype){
	struct memory *program;

	program = calloc(1, sizeof(struct memory));
	if(program== NULL) _mem_allocation_error(__func__);

	program->type = ttype;
	program->stackSize = START_STACK_SIZE;
	program->programStack = calloc(program->stackSize, sizeof(char));

	if(program->programStack == NULL)_mem_allocation_error(__func__);


	return program;
}


void exec (struct memory *program){
	while(program->pc <= program->programSize){

		//if(debug)printf("[%i::%i::%c] <%i> = %i\n", program->cyclesNumber, program->pc,program->programCode[program->pc],program->accumulator, program->programStack[program->accumulator]);

		switch(program->programCode[program->pc]){
			//+:aumenta cella di memoria puntata dall'accumulatore 
			case T_ADD:
				INC_CURRENT_STACK_CELL(program);
				break;
			//-: diminuisce cella di memoria puntata dall'accumulatore
			case T_SUB:
				DEC_CURRENT_STACK_CELL(program);
				
				break;
			//>: aumenta accumulatore
			case T_MOV_R:
				/* Controlla che la memoria allocata sia sufficente, se no rialloca e incrementa*/
				if(GET_STACK_OFFSET(program) >= (program->stackSize-1)){

					program->programStack=realloc(program->programStack, program->stackSize * DOUBLING_CONST);
					memset(program->programStack +  program->stackSize, 0, program->stackSize - 1);

					if(program->programStack==0)_mem_allocation_error(__func__);

					program->stackSize *= DOUBLING_CONST;
				}
				program->accumulator++;
				break;
			//<: diminuisce accumulatore
			case T_MOV_L:
				/*Se è a 0 ignora*/
				if(program->accumulator>=1)
					program->accumulator--;
				else
					perror("WARNING: decrementing accumulator while is zero.\n");
				break;
			//. :salva nel buffer dell'output
			case T_PRINT:

				putchar(GET_CURRENT_STACK_CELL(program));

				break;
			//, :Scrive l'output corrente, e lo bufferizza;salva nella cella puntata dall'acumulatore un carattere
			case T_INPUT:
				ASSIGN_TO_CURRENT_STACK_CELL(program, getchar());
				fflush(stdin);
				break;
			case T_WHILE_START:

				if(GET_CURRENT_STACK_CELL(program)==0){
					int brackets=0;

					while( program->pc < program->programSize){
						
						if(program->programCode[++program->pc] == T_WHILE_START)
							brackets++;

						else if(program->programCode[program->pc] == T_WHILE_END){
							if(brackets==0){
								break;
							}
							else
								brackets--;
						}
					}
				}
					if( program->pc == program->programSize && program->programCode[program->pc] != T_WHILE_END){
						fprintf(stderr, "<CRITICAL:%s>\n Unexpected end of file. Maybe missing an %c?\n", __func__ ,T_WHILE_END);
						clean_exit(program, EXIT_FAILURE);
					}

				break;
			case T_WHILE_END:
				if(GET_CURRENT_STACK_CELL(program)!=0){
					int brackets=0;
					while(program->pc > 0){
						
						if(program->programCode[--program->pc]==T_WHILE_END)
							brackets++;
							
						else if(program->programCode[program->pc]==T_WHILE_START){
							if(brackets==0)
								break;	
							else
								brackets--;
						}
					}
				}
				if( program->pc == 0 && program->programCode[program->pc] != T_WHILE_START){
						fprintf(stderr, "<CRITICAL:%s>\n Unexpected %c.\n", __func__ ,T_WHILE_END);
						clean_exit(program, EXIT_FAILURE);
					}
					
				break;
			default:
				fprintf(stderr, "<WARNING:%s>\n Unexpected %c, something weird is happening here.\n", __func__ , program->programCode[program->pc]);
				break;
			}
			
			program->cyclesNumber++; //incrementa numero di cicli
			program->pc++; //incrementa program counter
	
		}
}
/**
* Increment the stack cell that is currently pointed by the accumulator
*
*/
void increment_current_stack_cell(struct memory *program){
	void * current_stack = program->programStack+GET_STACK_OFFSET(program);
	u32 value = GET_CURRENT_STACK_CELL(program) ;

	switch(program->type){
		case BIT8:
			if(value >= UINT8_MAX){
				*((u8 *)current_stack) = 0;
				break;
			}
			(*((u8 *)current_stack))++;
			break;

		case BIT16:
			if(value >= UINT16_MAX){
				*((u16 *)current_stack) = 0;
				break;
			}
			(*((u16 *)current_stack))++;
			break;

		case BIT32:
			if(value >= UINT32_MAX){
				*((u32 *)current_stack) = 0;
				break;
			}
			(*((u32 *)current_stack))++;
			break;
			break;
		default:
			fprintf(stderr, "%s:Error, programType = %i\n", __func__, program->type);
			clean_exit(program, EXIT_FAILURE);
			break;
	}

}
/**
* decrement the stack cell that is currently pointed by the accumulator
*
*/
void decrement_current_stack_cell(struct memory *program){
	void * current_stack = GET_CURRENT_STACK_PTR(program);
	u32 value = GET_CURRENT_STACK_CELL(program) ;

	switch(program->type){
		case BIT8:
			if(value == 0){
				*((u8 *)current_stack) = UINT8_MAX;
				break;
			}
			(*((u8 *)current_stack))--;
			break;

		case BIT16:
			if(value == 0){
				*((u16 *)current_stack) = UINT16_MAX;
				break;
			}
			(*((u16 *)current_stack))--;
			break;

		case BIT32:
			if(value == 0){
				*((u32 *)current_stack) = UINT32_MAX;
				break;
			}
			(*((u32 *)current_stack))--;
			break;
		default:
			fprintf(stderr, "%s:Error, programType = %i\n", __func__, program->type);
			clean_exit(program, EXIT_FAILURE);
			break;
	}

}
/*Return current stack cell value, 32 for easyness*/
u32 get_current_stack_cell(struct memory *program){
	void * current_stack = GET_CURRENT_STACK_PTR(program);
	switch(program->type){
		case BIT8:
			return (u32)*((u8 *)current_stack);
		case BIT16:
			return (u32)*((u16 *)current_stack);
		
		case BIT32:
			return (u32)*((u16 *)current_stack);
		default:
			fprintf(stderr, "%s:Error, programType = %i\n", __func__, program->type);
			clean_exit(program, EXIT_FAILURE);
		
	}
	return 0;
}

void assign_to_current_stack_cell(struct memory *program, u32 value){
	void * current_stack = GET_CURRENT_STACK_PTR(program);

	switch(program->type){
		case BIT8:
			 *((u8 *)current_stack) = (u8) value;
			 break;
		case BIT16:
			 *((u16 *)current_stack) = (u16) value;
			 break;
		case BIT32:
			*((u32 *)current_stack) = (u32) value;
			break;
		default:
			fprintf(stderr, "%s:Error, programType = %i\n", __func__, program->type);
			clean_exit(program, EXIT_FAILURE);
			break;
	}	
}

void clean_exit(struct memory *program, int exit_type){

	free(program->programStack);
	free(program->programCode);
	free(program);
	exit(exit_type);
}


