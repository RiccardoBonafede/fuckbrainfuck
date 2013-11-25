#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1

int main(int argc,char *argv[]){
	FILE *progInput;
	char progByte[100000], c=0;
	char *progOutPut;
	int o=0;
	unsigned char *progStack;
	int progIndex=0, progDataIndex=0, stacklen=0, outlen=0, datalen, i=0;
	int in=0;
	int errors=0, whitechars=0;
	char deb=0;
	if(argc<2){
		fprintf(stderr, "Usage: brainfuck <progfile>\n");
		exit(0);
	};
	if(argc>=3 && !strcmp(argv[2], "--debug"))
		deb=TRUE;
	if((progInput=fopen(argv[1], "r"))==NULL){
		fprintf(stderr, "Errore:inpossibile aprire file %s\n", argv[1]);
		exit(0);
	};
	i=0;
	while(!feof(progInput)){
		
		fscanf(progInput, "%c", &c);
		if(c=='+' || c=='-' || c=='<' || c=='>' || c=='.' || c==',' || c=='[' || c==']'){
			progByte[i++]=c;
			
		}

	}
	fclose(progInput);
	if(deb)
	printf("[*]Prog data:\n%s\n[*]End prog data\n[*]starting Program\n", progByte);

	
	progStack=(char *)malloc(++stacklen);
	progOutPut=(char *)malloc(++outlen);
	datalen=strlen(progByte);

	//main loop
	while(1){
		if(progDataIndex>(datalen-1))
				break;
		if(deb)printf("[%i::%c] <%i> = %i\n", o, progByte[progDataIndex],progIndex, progStack[progIndex]);
		switch(progByte[progDataIndex]){
			//+:aumenta cella di memoria puntata dall'accumulatore 
			case '+':
				if(progStack[progIndex]<=255)
					progStack[progIndex]++;
				else	
					errors++;
				break;
			//-: diminuisce cella di memoria puntata dall'accumulatore
			case '-':
				if(progStack[progIndex]>0)
					progStack[progIndex]--;
				else
					errors++;
				break;
			//>: aumenta accumulatore
			case '>':
				if(progIndex<(stacklen-1))
					progIndex++;
				else{
					progStack=(char *)realloc(progStack, ++stacklen);
					
					progIndex++;
					progStack[progIndex]=0;
				}
				break;
			//<: diminuisce accumulatore
			case '<':
				if(progIndex>=1)
					progIndex--;
				break;
			//. :salva nel buffer dell'output
			case '.':
				progOutPut=(char *)realloc(progOutPut, ++outlen);
				progOutPut[outlen-1]=0;
				progOutPut[outlen-2]=progStack[progIndex];
				if(!deb)putchar(progStack[progIndex]);
				break;
			//, :Scrive l'output corrente, e lo bufferizza;salva nella cella puntata dall'acumulatore un carattere
			case ',':
				printf("%s", progOutPut);
				outlen=1;
				free(progOutPut);
				progOutPut=(char *)malloc(outlen);

				//if((progStack[progIndex]=getchar()) == '\n')
					progStack[progIndex]=getchar();
				break;
			case '[':
				if(progStack[progIndex]==0)
					while( 1){
						
						if(progByte[++progDataIndex]==']' & in==0)
							break;
						else if(progByte[progDataIndex]==']' & in>0)
							in--;
						if(progByte[progDataIndex]=='[')
							in++;
						if(progDataIndex>=datalen){
							errors++;
							break;
						}
					}
				in=0;
				break;
			case ']':
				if(progStack[progIndex]!=0)
					while(1){
						if(progByte[--progDataIndex]=='[' & in==0)
							break;
						else if(progByte[progDataIndex]=='[' & in>0)
							in--;
						if(progByte[progDataIndex]==']')
							in++;
						if(progDataIndex<=0){
							errors++;
							break;
						}
					}
				
					in=0;
					
				break;
			default:
				whitechars++;
				break;
			}
			
			
			
			o++;
			progDataIndex++;
	
		}
		progOutPut[outlen]=0;
		if(deb)printf("[*]Data len:%i\n[*]white chars:%i\n[*]errors:%i\n[*]action:%i\n", datalen, whitechars, errors,o);
		if(deb)printf("[*]Program output (%i bytes)\n", outlen);
		if(deb)for(i=0;i<outlen;i++)
			printf("%c", progOutPut[i]);
		if(deb)printf("\n[*]End of execution\n");
		free(progStack);
		free(progOutPut);
}
