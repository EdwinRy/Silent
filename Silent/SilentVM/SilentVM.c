#include "SilentVM.h"
#define BYTECODE_HALT 0
#define BYTECODE_GOTO 1
#define BYTECODE_CALL 2

#define BYTECODE_CLEAR_MEMORY 3
#define BYTECODE_CLEAR_STACK 4

#define BYTECODE_PUSH_BYTE 5
#define BYTECODE_PUSH_INT 6
#define BYTECODE_PUSH_LONG 7
#define BYTECODE_PUSH_FLOAT 8
#define BYTECODE_PUSH_DOUBLE 9

#define BYTECODE_POP_BYTE 10
#define BYTECODE_POP4 11
#define BYTECODE_POP8 12

#define BYTECODE_STORE_BYTE 13
#define BYTECODE_STORE_INT 14
#define BYTECODE_STORE_LONG 15
#define BYTECODE_STORE_FLOAT 16
#define BYTECODE_STORE_DOUBLE 17

#define BYTECODE_LOAD_BYTE 18
#define BYTECODE_LOAD_INT 19
#define BYTECODE_LOAD_LONG 20
#define BYTECODE_LOAD_FLOAT 21
#define BYTECODE_LOAD_DOUBLE 22

#define BYTECODE_ADD_BYTE 23
#define BYTECODE_ADD_INT 24
#define BYTECODE_ADD_LONG 25
#define BYTECODE_ADD_FLOAT 26
#define BYTECODE_ADD_DOUBLE 27

#define BYTECODE_SUB_BYTE 28
#define BYTECODE_SUB_INT 29
#define BYTECODE_SUB_LONG 30
#define BYTECODE_SUB_FLOAT 31
#define BYTECODE_SUB_DOUBLE 32

#define BYTECODE_MUL_BYTE 33
#define BYTECODE_MUL_INT 34
#define BYTECODE_MUL_LONG 35
#define BYTECODE_MUL_FLOAT 36
#define BYTECODE_MUL_DOUBLE 37

#define BYTECODE_DIV_BYTE 38
#define BYTECODE_DIV_INT 39
#define BYTECODE_DIV_LONG 40
#define BYTECODE_DIV_FLOAT 41
#define BYTECODE_DIV_DOUBLE 42

#define BYTECODE_BYTE_TO_INT 43
#define BYTECODE_BYTE_TO_LONG 44
#define BYTECODE_BYTE_TO_FLOAT 45
#define BYTECODE_BYTE_TO_DOUBLE 46

#define BYTECODE_INT_TO_BYTE 47
#define BYTECODE_INT_TO_FLOAT 48
#define BYTECODE_INT_TO_LONG 49
#define BYTECODE_INT_TO_DOUBLE 50

#define BYTECODE_FLOAT_TO_INT 51
#define BYTECODE_FLOAT_TO_DOUBLE 52

#define BYTECODE_SMALLER_THAN 53
#define BYTECODE_BIGGER_THAN 54
#define BYTECODE_EQUAL 55

#define BYTECODE_IF 56
#define BYTECODE_IF_NOT 57


SilentObject * createSilentObject(char * data)
{
	SilentObject* object = malloc(sizeof(SilentObject));
	object->data = data;
	return object;
}

SilentMemory* createSilentMemory(int storageSize, int stackSize)
{
	SilentMemory* memory = malloc(sizeof(SilentMemory));
	memory->stack = malloc(stackSize);
	memory->storage = malloc(storageSize);
	memory->stackPointer = 0;
	memory->storagePoiner = 0;
}

SilentThread * createSilentThread(char * bytecode, SilentMemory * memory)
{
	SilentThread* thread = malloc(sizeof(SilentThread));
	thread->bytecode = bytecode;
	thread->memory = memory;
	thread->programCounter = 0;
	thread->bytecodeLength = strlen(bytecode);
	return thread;
}

SilentVM * createSilentVM(SilentThread * thread, int numberOfThreads)
{
	SilentVM* vm = malloc(sizeof(SilentVM));
	vm->threads = malloc(sizeof(SilentThread*)*numberOfThreads);
	thread->threadID = 0;
	vm->threads[0] = thread;
	vm->threadPointer = 1;
	vm->defaultThread = 0;
	return vm;
}


void deleteSilentObject(SilentObject * object)
{
	free(object);
}

void deleteSilentMemory(SilentMemory * memory)
{
	free(memory->stack);
	free(memory->storage);
	free(memory);
}

void deleteSilentThread(SilentThread * thread)
{
	deleteSilentMemory(thread->memory);
	free(thread);
}

void deleteSilentVM(SilentVM * vm)
{
	deleteSilentThread(vm->threads);
	free(vm);
}



void loadProgramToSilentVM(SilentVM * vm, char * program)
{
	vm->bytecode = program;
}

void attachThreadToSilentVM(SilentVM * vm, SilentThread * thread)
{
	if (vm->threadPointer != 0) 
	{
		for (unsigned int i = 0; i < vm->threadPointer; i++) 
		{
			if (vm->threads[i] == NULL) 
			{
				thread->threadID = i;
				vm->threads[i] = thread;
				break;
			}
		}
		vm->threads[vm->threadPointer] = thread;
		thread->threadID = vm->threadPointer;
		vm->threadPointer++;
	}

	else 
	{
		vm->threads[vm->threadPointer] = thread;
		thread->threadID = vm->threadPointer;
		vm->threadPointer++;
	}
}

void loadProgramToSilentThread(SilentThread * thread, char * program)
{
	thread->bytecode = program;
}

void detachSilentThread(SilentVM * vm, unsigned int threadID)
{
	vm->threads[threadID] = NULL;
}

void SetDefaultSilentThread(SilentVM * vm, unsigned int threadID)
{
	vm->defaultThread = threadID;
}

void executeSilentThread(SilentVM * vm, unsigned int threadID)
{
	SilentThread* thread = vm->threads[threadID];
	thread->running = 1;

	int val;
	float val2;
	long val3;
	double val4;

	while (thread->running)
	{

		switch (thread->bytecode[thread->programCounter])
		{
		case BYTECODE_HALT:
			thread->running = 0;
			/*
			memset(thread->memory->storage, 0, thread->memory->storagePoiner);
			thread->memory->storagePoiner = 0;
			memset(thread->memory->stack, 0, thread->memory->stackPointer);
			thread->memory->stackPointer = 0;
			*/
			break;

		case BYTECODE_GOTO: 
			thread->programCounter = *((unsigned int*)(&vm->bytecode[++vm->threadPointer]));
			vm->threadPointer--;
			break;

		case BYTECODE_CALL://
			//implement native function calls
			break;



		case BYTECODE_CLEAR_MEMORY: //
			memset(thread->memory->storage, 0, thread->memory->storagePoiner);
			thread->memory->storagePoiner = 0;
			break;

		case BYTECODE_CLEAR_STACK: 
			memset(thread->memory->stack, 0, thread->memory->stackPointer);
			thread->memory->stackPointer = 0;
			break;



		case BYTECODE_PUSH_BYTE: 
			thread->memory->stack[thread->memory->stackPointer++] =
				thread->bytecode[++thread->programCounter];
			break;

		case BYTECODE_PUSH_INT:
			memcpy(thread->memory->stack + thread->memory->stackPointer,
				thread->bytecode + (++thread->programCounter),
				4);
			thread->programCounter += 3;
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_PUSH_LONG://
			break;

		case BYTECODE_PUSH_FLOAT://
			memcpy(thread->memory->stack + thread->memory->stackPointer,
				thread->bytecode + (++thread->programCounter),
				4);
			thread->programCounter += 3;
			thread->memory->stackPointer += 4;
			break;
		
		case BYTECODE_PUSH_DOUBLE://
			break;



		case BYTECODE_POP_BYTE:
			thread->memory->stackPointer--;
			break;

		case BYTECODE_POP4:
			thread->memory->stackPointer -= 4;
			break;

		case BYTECODE_POP8://
			break;


		case BYTECODE_STORE_BYTE://
			thread->memory->storage[thread->memory->storagePoiner]
				= createSilentObject(thread->bytecode[++thread->programCounter]);
			thread->memory->storagePoiner++;
			break;

		case BYTECODE_STORE_INT://
			thread->memory->storage[thread->memory->storagePoiner]
				= createSilentObject((int*)(thread->bytecode + (++thread->programCounter)));
			thread->memory->storagePoiner++;
			break;

		case BYTECODE_STORE_LONG://
			break;

		case BYTECODE_STORE_FLOAT://
			thread->memory->storage[thread->memory->storagePoiner]
				= createSilentObject((float*)(thread->bytecode + (++thread->programCounter)));
			break;

		case BYTECODE_STORE_DOUBLE://
			break;



		case BYTECODE_LOAD_BYTE://
			thread->memory->stack[thread->memory->stackPointer++] =
				thread->memory->storage[thread->bytecode[++thread->programCounter]];
			break;

		case BYTECODE_LOAD_INT://
			break;

		case BYTECODE_LOAD_LONG://
			break;

		case BYTECODE_LOAD_FLOAT://
			break;

		case BYTECODE_LOAD_DOUBLE://
			break;



		case BYTECODE_ADD_BYTE:
			thread->memory->stackPointer -= 2;
			thread->memory->stack[thread->memory->stackPointer++] =
				thread->memory->stack[thread->memory->stackPointer] + 
				thread->memory->stack[thread->memory->stackPointer + 1];
			thread->memory->stackPointer++;
			break;

		case BYTECODE_ADD_INT:
			thread->memory->stackPointer -= 8;
			thread->memory->stack[thread->memory->stackPointer] =
				(*((int*)(&thread->memory->stack[thread->memory->stackPointer])) +
					*((int*)(&thread->memory->stack[thread->memory->stackPointer + 4])));
			thread->memory->stackPointer += 4;

			break;

		case BYTECODE_ADD_LONG://
			break;

		case BYTECODE_ADD_FLOAT://
			thread->memory->stackPointer -= 8;
			thread->memory->stack[thread->memory->stackPointer] =
				(*((float*)(&thread->memory->stack[thread->memory->stackPointer])) +
					*((float*)(&thread->memory->stack[thread->memory->stackPointer + 4])));
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_ADD_DOUBLE://
			break;



		case BYTECODE_SUB_BYTE:
			thread->memory->stackPointer -= 2;
			thread->memory->stack[thread->memory->stackPointer++] =
				thread->memory->stack[thread->memory->stackPointer] -
				thread->memory->stack[thread->memory->stackPointer + 1];
			thread->memory->stackPointer++;
			break;

		case BYTECODE_SUB_INT:
			thread->memory->stackPointer -= 8;
			thread->memory->stack[thread->memory->stackPointer] =
				(*((int*)(&thread->memory->stack[thread->memory->stackPointer])) -
					*((int*)(&thread->memory->stack[thread->memory->stackPointer + 4])));
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_SUB_LONG://
			break;

		case BYTECODE_SUB_FLOAT://
			break;

		case BYTECODE_SUB_DOUBLE://
			break;



		case BYTECODE_MUL_BYTE:
			thread->memory->stackPointer -= 2;
			thread->memory->stack[thread->memory->stackPointer++] =
				thread->memory->stack[thread->memory->stackPointer] *
				thread->memory->stack[thread->memory->stackPointer + 1];
			thread->memory->stackPointer++;
			break;

		case BYTECODE_MUL_INT:
			thread->memory->stackPointer -= 8;
			val = (*((int*)(thread->memory->stack + thread->memory->stackPointer))) *
				(*((int*)(thread->memory->stack + thread->memory->stackPointer + 4)));
			memcpy(thread->memory->stack + thread->memory->stackPointer,
				&val,
				4);
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_MUL_LONG://
			break;

		case BYTECODE_MUL_FLOAT://
			thread->memory->stackPointer -= 8;
			val2 = (*((int*)(thread->memory->stack + thread->memory->stackPointer))) *
				(*((int*)(thread->memory->stack + thread->memory->stackPointer + 4)));
			memcpy(thread->memory->stack + thread->memory->stackPointer,
				&val2,
				4);
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_MUL_DOUBLE://
			break;



		case BYTECODE_DIV_BYTE:
			thread->memory->stackPointer -= 2;
			thread->memory->stack[thread->memory->stackPointer++] =
				thread->memory->stack[thread->memory->stackPointer] /
				thread->memory->stack[thread->memory->stackPointer + 1];
			thread->memory->stackPointer++;
			break;

		case BYTECODE_DIV_INT:
			thread->memory->stackPointer -= 8;
			val = (*((int*)(thread->memory->stack + thread->memory->stackPointer))) /
				(*((int*)(thread->memory->stack + thread->memory->stackPointer + 4)));
			memcpy(thread->memory->stack + thread->memory->stackPointer,
				&val,
				4);
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_DIV_LONG://
			break;

		case BYTECODE_DIV_FLOAT://
			thread->memory->stackPointer -= 8;
			val2 = (*((float*)(thread->memory->stack + thread->memory->stackPointer))) /
				(*((float*)(thread->memory->stack + thread->memory->stackPointer + 4)));
			memcpy(thread->memory->stack + thread->memory->stackPointer,
				&val2,
				4);
			thread->memory->stackPointer += 4;
			break;

		case BYTECODE_DIV_DOUBLE://
			break;



		case BYTECODE_BYTE_TO_INT://
			break;

		case BYTECODE_BYTE_TO_LONG://
			break;

		case BYTECODE_BYTE_TO_FLOAT://
			break;

		case BYTECODE_BYTE_TO_DOUBLE://
			break;



		case BYTECODE_INT_TO_BYTE://
			break;

		case BYTECODE_INT_TO_FLOAT://
			break;

		case BYTECODE_INT_TO_LONG://
			break;

		case BYTECODE_INT_TO_DOUBLE://
			break;



		case BYTECODE_FLOAT_TO_INT://
			break;

		case BYTECODE_FLOAT_TO_DOUBLE://
			break;



		case BYTECODE_SMALLER_THAN://
			break;

		case BYTECODE_BIGGER_THAN://
			break;

		case BYTECODE_EQUAL:
			break;



		case BYTECODE_IF://
			break;

		case BYTECODE_IF_NOT://
			break;
		}
		thread->programCounter++;
	}

}

void startSilentVM(SilentVM * vm)
{
	executeSilentThread(vm, vm->defaultThread);
}