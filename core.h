#define START_STACK_SIZE 255

#ifndef DOUBLING_CONST
	#define DOUBLING_CONST 2 
#endif

#define ARRAY_START_LENGHT 8
#define OUTPUT_BUFFER_SIZE 

#define VALID_CHARACTERS "+-.,[]<>"

typedef enum bfInstruction{
	T_MOV_R = '>',
	T_MOV_L = '<',
	T_ADD = '+',
	T_SUB = '-',
	T_WHILE_START = '[',
	T_WHILE_END = ']',
	T_PRINT = '.', 
	T_INPUT = ','
};

struct memory_8b {
	int pc;
	unsigned int accumulator;
	unsigned long long int cyclesNumber;
	size_t programSize;
	char *programCode;
	unsigned char *programStack;
	size_t stackSize;
};

struct memory_8b *load_file_8b(char *filename, struct memory_8b *memory);