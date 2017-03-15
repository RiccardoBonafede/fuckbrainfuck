#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "main.h"
#include "utils.h"
#include "core.h"
#include "args.h"


/*void push(struct STACK *s, int ele);
int pop(struct STACK *s);*/
	
void _mem_allocation_error();
size_t insertIntoArray(void ** arrayPtr, void * bytes, size_t bytes_to_write,size_t ndata ,size_t array_size);




int main(int argc,char *argv[]){
	
	int errors=0, whitechars=0;
	char debug;
	struct memory *program;
	struct options *opt;

	opt = parse_args(argc, argv);
	
	program = prepare_memory(opt);

	program = load_file(opt->filename, program);

	//if there is no code we finish!
	if(program->programSize <= 0){
		clean_exit(program, EXIT_SUCCESS);
	}

	if(opt->debug == 1)
		printf("<debug>Prog data(%lu):\n%s\n<debugUG>End prog data\n<debugUG>starting Program\n",program->programSize, program->programCode);

	//inizializing the "stack"
	//main loop
	exec(program);

	fflush(stdout);
		//progOutPut[outlen]=0; //termina buffer

	if(opt->debug){
		printf("[*]white chars:%i\n[*]errors:%i\n[*]action:%llu\n" , whitechars, errors, program->cyclesNumber++);
		printf("\n[*]End of execution\n");
		}
	clean_exit(program, EXIT_SUCCESS);
}

