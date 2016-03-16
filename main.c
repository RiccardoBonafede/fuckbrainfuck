#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/*void push(struct STACK *s, int ele);
int pop(struct STACK *s);*/
	
void _mem_allocation_error();


int main(int argc,char *argv[]){
	printf("FROSINONE CULONE. \n"); //mcillo loves printf
	FILE *progInput;
	
	char *progByte, *progOutPut;
	unsigned char *progStack;
	int progIndex=0, pc=0, stacklen=0, outlen=0, i=0;
	int errors=0, whitechars=0, o=0, programSize=0;
	char deb=0;


	//controlla se ci sono tutti gli argomenti
	if(argc<2){
		fprintf(stderr, "Usage: brainfuck <progfile> (--debug)\n");
		exit(0);
	};
	//guarda se è abilitata l'opzione "debug"
	if(argc>=3 && !strcmp(argv[2], "--debug"))
		deb=1;
	//prova ad aprire il file. Se fallisce esce
	if((progInput=fopen(argv[1], "r"))==NULL){
		fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
		exit(0);
	};
	//inizializza memoria per programma
	progByte=(char *)malloc(sizeof(char));
	
	if(progByte==0)_mem_allocation_error();
	
	
	char c=0;
	//Legge il codice dal file
	while(!feof(progInput)){

		fscanf(progInput, "%c", &c);
		

		if(c=='+' || c=='-' || c=='<' || c=='>' || c=='.' || c==',' || c=='[' || c==']'){	//ignora tutto il resto
			progByte=(char *)realloc(progByte, (++i)*sizeof(char));
			if(progByte==0)_mem_allocation_error();

			progByte[i-1]=c;
		}
		c=0;

	}
	
	fclose(progInput);

	programSize=i;
	i=0;

	if(deb)printf("[*]Prog data(%i):\n%s\n[*]End prog data\n[*]starting Program\n",programSize, progByte);

	
	progStack=(char *)calloc(++stacklen, sizeof(char));
	progOutPut=(char *)calloc(++outlen, sizeof(char));

	if(!progStack || !progOutPut)_mem_allocation_error();
	
	//main loop
	while(1){
		if(pc>(programSize-1))
				break;

		if(deb)printf("[%i::%i::%c] <%i> = %i\n", o, pc,progByte[pc],progIndex, progStack[progIndex]);

		switch(progByte[pc]){
			//+:aumenta cella di memoria puntata dall'accumulatore 
			case '+':
				if(progStack[progIndex]>=255)
					errors++;
				progStack[progIndex]++;
				
				break;
			//-: diminuisce cella di memoria puntata dall'accumulatore
			case '-':
				if(progStack[progIndex]<0)
					errors++;
				progStack[progIndex]--;
				
				break;
			//>: aumenta accumulatore
			case '>':

				if(progIndex<(stacklen-1))
					progIndex++;
				else{
					progStack=(char *)realloc(progStack, stacklen*2);
					if(progStack==0)_mem_allocation_error();

					
					stacklen*=2;
					progIndex++;
					progStack[progIndex]=0;
				}

				break;
			//<: diminuisce accumulatore
			case '<':
				if(progIndex>=1)
					progIndex--;
				else
					errors++;
				break;
			//. :salva nel buffer dell'output
			case '.':

				progOutPut=(char *)realloc(progOutPut, ++outlen); //rialloca e controlla eventuali errori
				if(progOutPut==0)_mem_allocation_error();

				progOutPut[outlen-1]=0;
				progOutPut[outlen-2]=progStack[progIndex];

				if(!deb)putchar(progStack[progIndex]);
				break;
			//, :Scrive l'output corrente, e lo bufferizza;salva nella cella puntata dall'acumulatore un carattere
			case ',':
				if(outlen>0 && deb){

					printf("Program asks \"%s\" >", progOutPut);
					free(progOutPut);

					outlen=1;
					progOutPut=(char *)malloc(outlen);
					if(progOutPut==0)_mem_allocation_error();
				}
					progStack[progIndex]=getchar();
					fflush(stdin);
				break;
			case '[':

				if(progStack[progIndex]==0){
					int brackets=0;

					while(1){
						
						if(progByte[++pc]=='[')
							brackets++;

						else if(progByte[pc]==']'){
							if(brackets==0){
								break;
							}
							else
								brackets--;
						}
					}
				}	
				break;
			case ']':
				if(progStack[progIndex]!=0){
					int brackets=0;
				
					while(1){
						
						if(progByte[--pc]==']')
							brackets++;
							
						else if(progByte[pc]=='['){
							if(brackets==0)
								break;	
							else
								brackets--;
						}
				}
			}
				break;
			default:
				whitechars++; //Questo non dovrebbe accadere
				break;
			}
			
			o++; //incrementa numero di cicli
			pc++; //incrementa program counter
	
		}

		progOutPut[outlen]=0; //termina buffer

		if(deb){
			printf("[*]white chars:%i\n[*]errors:%i\n[*]action:%i\n" , whitechars, errors,o);
			printf("[*]Program output (%i bytes)\n", outlen);
			for(i=0;i<outlen;i++)
				printf("%i ", progOutPut[i]);
			printf("\n[*]End of execution\n");
		}
		free(progStack);
		free(progByte);
		free(progOutPut);
}

void _mem_allocation_error(){
		fprintf(stderr, "%s\n", "error while allocating new memory\n");
		exit(-1);
}

