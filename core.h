#include <stdint.h>

#define START_STACK_SIZE 1024

#ifndef DOUBLING_CONST
	#define DOUBLING_CONST 2 
#endif

#define ARRAY_START_LENGHT 24
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
enum programType{
	BIT8 = 1,
	BIT16,
	BIT32
};
struct brackets {
	unsigned int open;
	unsigned int close;
};
/*union cellmem{
	char bit8
}*/
struct memory{
	unsigned int pc;
	unsigned int accumulator;
	unsigned long long int cyclesNumber;
	size_t programSize;
	char *programCode;
	void *programStack;
	size_t stackSize;
	enum programType type;
	//struct brackets *bracketsArray;
	//unsigned int nOfBrackets;
};

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;



struct memory *load_file(char *filename, struct memory *memory);
void exec (struct memory *program);
struct memory *prepare_memory(enum programType ttype);
void clean_exit(struct memory *program, int exit_type);