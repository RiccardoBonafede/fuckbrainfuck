#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "utils.h"
#include "core.h"



/*void push(struct STACK *s, int ele);
int pop(struct STACK *s);*/
	
void _mem_allocation_error();
size_t insertIntoArray(void ** arrayPtr, void * bytes, size_t bytes_to_write,size_t ndata ,size_t array_size);




int main(int argc,char *argv[]){
	
	int errors=0, whitechars=0;
	char deb;
	struct memory_8b *program;

	//controlla se ci sono tutti gli argomenti
	if(argc<2){
		fprintf(stderr, "Usage: %s <progfile> (--debug)\n", argv[0]);
		exit(0);
	};
	//guarda se è abilitata l'opzione "debug"
	if(argc>=3 && !strcmp(argv[2], "--debug"))
		deb=1;
	else 
		deb=0;
	
	program = calloc(1, sizeof(struct memory_8b));
	if(program== NULL) _mem_allocation_error(__func__);

	program = load_file_8b(argv[1], program);

	if(program->programSize <= 0){
		free(program->programCode);
		free(program);

	}
	if(deb)printf("<DEBUG>Prog data(%i):\n%s\n<DEBUG>End prog data\n<DEBUG>starting Program\n",program->programSize, program->programCode);

	program->stackSize = START_STACK_SIZE;
	program->programStack = calloc(program->stackSize, sizeof(char));

	if(program->programStack == NULL)_mem_allocation_error(__func__);

	
	//main loop
	while(program->pc <= program->programSize){

		if(deb)printf("[%i::%i::%c] <%i> = %i\n", program->cyclesNumber, program->pc,program->programCode[program->pc],program->accumulator, program->programStack[program->accumulator]);

		switch(program->programCode[program->pc]){
			//+:aumenta cella di memoria puntata dall'accumulatore 
			case T_ADD:
				if(program->programStack[program->accumulator]>=255){
					program->programStack[program->accumulator] = 0;
					break;
				}
				program->programStack[program->accumulator]++;
				
				break;
			//-: diminuisce cella di memoria puntata dall'accumulatore
			case T_SUB:
				if(program->programStack[program->accumulator]<=0){
					program->programStack[program->accumulator] = 255;
					break;
				}
				program->programStack[program->accumulator]--;
				
				break;
			//>: aumenta accumulatore
			case T_MOV_R:

				if(program->accumulator<(program->stackSize-1))
					program->accumulator++;
				else{
					program->programStack=realloc(program->programStack, program->stackSize*2);
					if(program->programStack==0)_mem_allocation_error(__func__);
					program->stackSize*=2;
					program->accumulator++;
					program->programStack[program->accumulator]=0;
				}

				break;
			//<: diminuisce accumulatore
			case T_MOV_L:
				if(program->accumulator>=1)
					program->accumulator--;
				else
					errors++;
				break;
			//. :salva nel buffer dell'output
			case T_PRINT:

				if(!deb)putchar(program->programStack[program->accumulator]);

				break;
			//, :Scrive l'output corrente, e lo bufferizza;salva nella cella puntata dall'acumulatore un carattere
			case T_INPUT:

				if(deb){
					printf("Program asks $");
				}

				program->programStack[program->accumulator]=getchar();
				fflush(stdin);
				break;
			case T_WHILE_START:

				if(program->programStack[program->accumulator]==0){
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
					if( program->pc == program->programSize && program->programCode[program->pc] != T_WHILE_END){
						fprintf(stderr, "<CRITICAL:%s>\n Unexpected end of file. Maybe missing an %c?\n", __func__ ,T_WHILE_END);
						exit(EXIT_FAILURE);
					}
				}	
				break;
			case T_WHILE_END:
				if(program->programStack[program->accumulator]!=0){
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
				if( program->pc == 0&& program->programCode[program->pc] != T_WHILE_START){
						fprintf(stderr, "<CRITICAL:%s>\n Unexpected %c.\n", __func__ ,T_WHILE_END);
						exit(EXIT_FAILURE);
					}
				break;
			default:
				fprintf(stderr, "<WARNING:%s>\n Unexpected %c, something weird is happening here.\n", __func__ , program->programCode[program->pc]);
				break;
			}
			
			program->cyclesNumber++; //incrementa numero di cicli
			program->pc++; //incrementa program counter
	
		}
		fflush(stdout);
		//progOutPut[outlen]=0; //termina buffer

		if(deb){
			printf("[*]white chars:%i\n[*]errors:%i\n[*]action:%i\n" , whitechars, errors, program->cyclesNumber++);
			printf("\n[*]End of execution\n");
		}
		free(program->programStack);
		free(program->programCode);
		free(program);
}

