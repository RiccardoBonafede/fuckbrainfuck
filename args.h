struct options
{
	enum programType type;
	char *filename;
	char debug; //for legacy
};

void help_exit(char *program_name, int exit_code, void *toFree);
struct options* parse_args(int argc, char *argv[]);
