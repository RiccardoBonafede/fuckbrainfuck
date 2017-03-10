#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "core.h"
#include "args.h"

#define ARGUMENTS "b:h"

/*bitwidth possible values*/
#define BITWIDTH8 "8"
#define BITWIDTH16 "16"
#define BITWIDTH32 "32"

#define DEFAULT_BITWIDTH BIT8


struct options* parse_args(int argc, char *argv[]){
	extern char *optarg;
    extern int optind, optopt;
    int c=0;
    struct options *opt = calloc (1, sizeof (struct options));

    while ((c = getopt(argc, argv, ARGUMENTS)) != -1) {
    	switch(c) {
    		case 'b':
    			if(opt->type != 0)
    				help_exit(argv[0], EXIT_FAILURE, (void *)opt);
    			//if is an accepted value
       			if(!strcmp(optarg, BITWIDTH8) ||  !strcmp(optarg, BITWIDTH16) || !strcmp(optarg, BITWIDTH32)){
       				char bit=atoi(optarg);
              bit /= 4;
       				//calculate log2(optarg)
       				while (bit >>= 1) opt->type++;

       			} else{
       				help_exit(argv[0], EXIT_FAILURE, (void *)opt);
       			}
 	       break;
    		case 'h':
    	    	help_exit(argv[0], EXIT_SUCCESS, (void *)opt);
    	    	break;
       		break;
    			case ':':
       		 	printf("-%c  need a filename!\n", optopt);
       		break;
    			case '?':
        		printf("Unknown arg %c\n", optopt);
        		help_exit(argv[0], EXIT_FAILURE, (void *)opt);
      		break;
    	}
	}

	/*Set Filename and check if the user as supplied one*/
	if(optind >= argc){
		free(opt);
		help_exit(argv[0], EXIT_FAILURE, (void *)opt);
	}
	opt->filename = argv[optind];

	if(opt->type == 0)
		opt->type = DEFAULT_BITWIDTH; 
	return opt;
}
/**
 * Print an help page and exit
 *
 * \args exit_code exit code
 */
void help_exit(char *program_name, int exit_code, void *toFree){
	const char *help = " FuckBrainFuck interpreter, 2017 \n " 
						"Usage: %s [options] file\n" 
						"Options:\n"
						"\t-b, --bitwidth <args> 	Cell size in bit, default 8. Accepted args are 8, 16, 32.\n"
						"\t-h, --help 				Show this help and exit\n";
	printf(help, program_name);
	free(toFree);
	exit(exit_code);

}
