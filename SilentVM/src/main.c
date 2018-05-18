#include "SilentVM.h"
#include "SilentLib.h"
#include <stdio.h>
char* readAllText(char* path)
{
    char* text;
	FILE *f;
	if((f = fopen(path,"r"))==NULL)
	{
		printf("File %s doesn't exist!\n",path);
		exit(-1);
	}
	fseek(f,0,SEEK_END);
	text = malloc(ftell(f));
	fseek(f,0,SEEK_SET);
	long count = 0;
	char c;
	while((c = fgetc(f))!=EOF){text[count++] = (char)c;}
    fclose(f);
    return text;
}

int main(int argc, char** argv)
{
	//if(argc<2){printf("SilentVM requires an input file\n");return(-1);}
    //char* inFile = argv[1];
    //char* program = readAllText(inFile); 


	char program[] = 
	{
		Goto,
		14,0,0,0,0,0,0,0,
		Push4,
		9,0,0,0,
		Push4,
		10,0,0,0,
		Halt
	};

	SilentMemory* memory = createSilentMemory(8000,8000);
	SilentGC* gc = createSilentGC(memory);
	SilentVM* vm = createSilentVM(memory,program,gc);

	silentVMStart(vm);

	printf("stack %i\n",*((int*)memory->stack));
	printf("stack ptr %i\n",memory->stackPointer);
	printf("stackT %i\n",*(memory->stackTypes));
	printf("stackTS %i\n",memory->stackTypePtr);

	deleteSilentGC(gc);
	deleteSilentMemory(memory);
	deleteSilentVM(vm);
    return 0;
}