#include "SilentVM.h"
#include <memory.h>

//Allocate memory for the progra
SilentMemory* createSilentMemory(int storageSize, int stackSize)
{
	SilentMemory* memory = malloc(sizeof(SilentMemory));
	memory->storage = malloc(sizeof(char*)*storageSize);
	memory->stack = malloc(stackSize);
	memory->storagePointer = 0;
	memory->stackPointer = 0;
	memory->functionPointer = 0;
	return memory;
}

//Create executing thread
SilentThread* createSilentThread(SilentMemory* memory, char* bytecode)
{
	SilentThread* thread = malloc(sizeof(SilentThread));
	thread->bytecode = bytecode;
	thread->running = 0;
	thread->programCounter = 0;
	thread->memory = memory;
	return thread;
}

//Delete program's allocated space
void deleteSilentMemory(SilentMemory * memory)
{
	free(memory->stack);
	free(memory->storage);
	free(memory);
}

//Delete program's thread
void deleteSilentThread(SilentThread * thread)
{
	free(thread);
}

void executeSilentThread(SilentThread * thread)
{
	long lreg = 0;
	int ireg = 0;
	char breg = 0;
	float freg = 0;
	double dreg = 0;
	thread->running = 1;
	while(thread->running)
	{
		switch(thread->bytecode[thread->programCounter])
		{
			//Stops the execution of the program
			case Halt:
				thread->running = 0;
				break;
			
			//Changes the program pointer based on bytecode
			case Goto:
				thread->programCounter++;
				thread->programCounter = 
					*((unsigned long*)(thread->bytecode + (thread->programCounter)));
				break;
			
			//Will be used to call library functions
			case Call: //Not yet implemented
				break;

			//case ClearMemory://
			//	for(int i = 0; i < thread->memory->storagePointer; i++)
			//	{free(thread->memory->storage);}
			//	thread->memory->storagePointer = 0;
			//	break;

			//case ClearStack://
			//	memset(thread->memory->stack, 0, thread->memory->stackPointer);
			//	thread->memory->stackPointer = 0;
			//	break;

			//Pushes 1 byte of data to the stack
			case Push1:
				thread->memory->stack[thread->memory->stackPointer++] = 
					thread->bytecode[++thread->programCounter];
				break;
				
			//Pushes 4 bytes of data to the stack
			case Push4:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						4);
				thread->programCounter += 3;
				thread->memory->stackPointer += 4;
				break;
			
			//Pushes 8 bytes of data to the stack
			case Push8:
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + (++thread->programCounter),
						8);
				thread->programCounter += 7;
				thread->memory->stackPointer += 8;
				break;

			//Pushes X (in bytecode) bytes of data to the stack
			case PushX:
				lreg = *((long*)(thread->bytecode + (++thread->programCounter)));
				memcpy(thread->memory->stack + thread->memory->stackPointer,
						thread->bytecode + 8 + thread->programCounter,
						lreg);
				thread->programCounter += (7+lreg);
				thread->memory->stackPointer += lreg;
				break;
			
			//Decreases the stack pointer by 1
			case Pop1:
				thread->memory->stackPointer--;
				break;
			
			//Decreases the stack pointer by 4
			case Pop4:
				thread->memory->stackPointer-=4;
				break;
					
			//Decreases the stack pointer by 8
			case Pop8:
				thread->memory->stackPointer-=8;
				break;

			//Decreases the stack pointer by X (in bytecode)
			case PopX:
				thread->memory->stackPointer-=
					*(long*)(thread->bytecode + (++thread->programCounter));
				thread->programCounter += 7;
				break;

			//Saves 1 byte from the stack to allocated space
			case Store1:
				memcpy(
					thread->memory->storage[
						*(long*)(thread->bytecode + (++thread->programCounter))
					],
				thread->memory->stack + (--thread->memory->stackPointer),
					1
				);
				thread->programCounter += 7;
				break;

			//Saves 4 bytes from the stack to allocated space
			case Store4:
				thread->memory->stackPointer -= 4;
				memcpy(
					thread->memory->storage[
						*(long*)(thread->bytecode + (++thread->programCounter))
					],
					thread->memory->stack + (thread->memory->stackPointer),
					4
				);
				thread->programCounter += 7;
				break;

			//Saves 8 bytes from the stack to allocated space
			case Store8:		
				thread->memory->stackPointer -= 8;
				memcpy(	
					thread->memory->storage[
						*(long*)(thread->bytecode + (++thread->programCounter))
					],
					thread->memory->stack + (thread->memory->stackPointer),
					8
				);
				thread->programCounter += 7;		
				break;
			//Saves X bytes from stack to allocated space
			case StoreX://untested
				lreg = *((long*)(thread->bytecode + (++thread->programCounter)));
				thread->programCounter+=7;
				memcpy(
					thread->memory->storage[
						*(long*)(thread->bytecode + (++thread->programCounter))
					],
					thread->memory->stack + (thread->memory->stackPointer),
						lreg);
				thread->programCounter += 7;
				thread->memory->stackPointer += lreg;
				break;
	
			//Copies 1 byte of data from storage onto the stack
			case Load1:
				memcpy
				(
					thread->memory->stack + (thread->memory->stackPointer++),
					thread->memory->storage[
							*(long*)(thread->bytecode + (++thread->programCounter))
					],
					1
				);
				thread->programCounter += 7;
				break;

			//Copies 4 bytes of data from storage onto the stack
			case Load4:
				memcpy
				(
					thread->memory->stack + (thread->memory->stackPointer),
					thread->memory->storage[
							*(long*)(thread->bytecode + (++thread->programCounter))
					],
					4
				);
				thread->memory->stackPointer += 4;
				thread->programCounter += 7;
				break;

			//Copies 8 bytes of data from storage onto the stack
			case Load8:
				memcpy
				(
					thread->memory->stack + (thread->memory->stackPointer),
					thread->memory->storage[
							*(long*)(thread->bytecode + (++thread->programCounter))
					],
					8
				);
				thread->memory->stackPointer += 8;
				thread->programCounter += 7;
				break;

			//Copies X bytes of data from storage onto the stack
			case LoadX:
				lreg = *((long*)(thread->bytecode + (++thread->programCounter)));
				thread->programCounter+=7;
				memcpy(
					thread->memory->storage[
						*(long*)(thread->bytecode + (++thread->programCounter))
					],
					thread->memory->stack + (thread->memory->stackPointer),
						lreg);
				thread->programCounter += 7;
				thread->memory->stackPointer += lreg;

				break;

			//Allocates 1 byte of data for the program
			case Alloc1:
				thread->memory->storage[thread->memory->storagePointer++]
					= malloc(1);
				break;

			//Allocates 4 bytes of data for the program
			case Alloc4:
				thread->memory->storage[thread->memory->storagePointer++]
					= malloc(4);
				break;

			//Allocates 8 bytes of data for the program
			case Alloc8:
				thread->memory->storage[thread->memory->storagePointer++]
					= malloc(8);
				break;

			//Allocates X bytes of data for the program
			case AllocX://untested
				thread->memory->storage[thread->memory->storagePointer++]
					= malloc(*(long*)(thread->bytecode + (++thread->programCounter)));
				thread->programCounter += 7;
				break;
			
			//Releases the lastly allocated storage
			case FREE:
				free(thread->memory->storage[--thread->memory->storagePointer]);
				break;

			/*
			case LoadPtr:
				break;

			case EditPtr1:
				break;
			*/

			//Adds together 2 bytes on the stack
			case AddByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) += 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Adds together 2 integers on the stack
			case AddInt:
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) += 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Adds together 2 longs on the stack
			case AddLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) += 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Adds together 2 floats on the stack
			case AddFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) += 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Adds together 2 doubles on the stack
			case AddDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) += 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);

				break;

			//Subtracts the last number from the second last number on the stack
			case SubByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) -= 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Subtracts the last number from the second last number on the stack
			case SubInt:
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) -= 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Subtracts the last number from the second last number on the stack
			case SubLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) -= 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Subtracts the last number from the second last number on the stack
			case SubFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) -= 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Subtracts the last number from the second last number on the stack
			case SubDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) -= 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);

				break;
			
			//Multiplies 2 bytes together
			case MulByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) *= 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Multiplies 2 integers together
			case MulInt:
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) *= 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Multiplies 2 longs together
			case MulLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) *= 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Multiplies 2 floats together
			case MulFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) *= 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Multiplies 2 doubles together
			case MulDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) *= 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Divides 2 bytes
			case DivByte:
				thread->memory->stackPointer--;
				*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) /= 
					*(char*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Divides 2 integers
			case DivInt:
				thread->memory->stackPointer-=4;
				*(int*)(thread->memory->stack + (thread->memory->stackPointer-4)) /= 
					*(int*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Divides 2 longs
			case DivLong:
				thread->memory->stackPointer-=8;
				*(long*)(thread->memory->stack + (thread->memory->stackPointer-8)) /= 
					*(long*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Divides 2 floats
			case DivFloat:
				thread->memory->stackPointer-=4;
				*(float*)(thread->memory->stack + (thread->memory->stackPointer-4)) /= 
					*(float*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Divides 2 doubles
			case DivDouble:
				thread->memory->stackPointer-=8;
				*(double*)(thread->memory->stack + (thread->memory->stackPointer-8)) /= 
					*(double*)(thread->memory->stack + thread->memory->stackPointer);
				break;

			//Byte to integer conversion
			case ByteToInt://untested
				breg = *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				ireg = (int)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&ireg, 
					4);
				thread->memory->stackPointer+=4;
				break;

			//Byte to long conversion
			case ByteToLong://untested
				breg =  *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				lreg = (long)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&lreg,
					8);
				thread->memory->stackPointer+=8;
				break;

			//Byte to float conversion
			case ByteToFloat://untested
				breg =  *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				freg = (float)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&freg,
					4);
				thread->memory->stackPointer+=4;
				break;

			//Byte to double conversion
			case ByteToDouble://untested
				breg =  *(char*)(thread->memory->stack + (thread->memory->stackPointer--));
				dreg = (double)breg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&dreg,
					8);
				thread->memory->stackPointer+=8;

				break;

			//Integer to byte conversion
			case IntToByte://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-=4));
				breg = (char)ireg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&breg,
					1);
				thread->memory->stackPointer++;

				break;

			//Integer to long conversion
			case IntToLong://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-=4));
				lreg = (long)ireg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer,
					&lreg,
					8);
				thread->memory->stackPointer+=8;
				break;
			
			//Integer to float conversion
			case IntToFloat://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-4));
				freg = (float)ireg;
				memcpy(
					thread->memory->stack + (thread->memory->stackPointer-4),
					&freg,
					4);
				break;

			//Integer to double conversion
			case IntToDouble://untested
				ireg =  *(int*)(thread->memory->stack + (thread->memory->stackPointer-4));
				dreg = (double)ireg;
				memcpy(
					thread->memory->stack + thread->memory->stackPointer-4,
					&freg,
					8);
				thread->memory->stackPointer+=4;

				break;

			//Float to integer conversion
			case FloatToInt://untested

				break;

			//Float to double conversion
			case FloatToDouble://untested
				thread->memory->stackPointer+=4;
				
				break;
			
			//Compare value of 2 bytes
			case SmallerThanByte:
				thread->memory->stackPointer--;
				if(*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) <
					*(char*)(thread->memory->stack + thread->memory->stackPointer))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 4 bytes
			case SmallerThanInt:
				thread->memory->stackPointer-=7;
				if(*(int*)(thread->memory->stack + thread->memory->stackPointer-1) < 
					*(int*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}			
				break;

			//Compare value of 2 8 bytes
			case SmallerThanLong:
				thread->memory->stackPointer-=15;
				if(*(long*)(thread->memory->stack + (thread->memory->stackPointer-1)) < 
					*(long*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 4 bytes
			case SmallerThanFloat:
				thread->memory->stackPointer-=7;
				if(*(float*)(thread->memory->stack + (thread->memory->stackPointer-1)) < 
					*(float*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 8 bytes
			case SmallerThanDouble:
				thread->memory->stackPointer-=15;
				if(*(double*)(thread->memory->stack + (thread->memory->stackPointer-1)) < 
					*(double*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 bytes
			case BiggerThanByte:
				thread->memory->stackPointer--;
				if(*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(char*)(thread->memory->stack + thread->memory->stackPointer))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 4 bytes
			case BiggerThanInt:
				thread->memory->stackPointer-=7;
				if(*(int*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(int*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 8 bytes
			case BiggerThanLong:
				thread->memory->stackPointer-=15;
				if(*(long*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(long*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 4 bytes
			case BiggerThanFloat:
				thread->memory->stackPointer-=7;
				if(*(float*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(float*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 8 bytes
			case BiggerThanDouble:
				thread->memory->stackPointer-=15;
				if(*(double*)(thread->memory->stack + (thread->memory->stackPointer-1)) > 
					*(double*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 bytes
			case EqualByte:
				thread->memory->stackPointer--;
				if(*(char*)(thread->memory->stack + (thread->memory->stackPointer-1)) == 
					*(char*)(thread->memory->stack + thread->memory->stackPointer))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 4 bytes
			case EqualInt:
				thread->memory->stackPointer-=7;
				if(*(int*)(thread->memory->stack + (thread->memory->stackPointer-1)) == 
					*(int*)(thread->memory->stack + thread->memory->stackPointer+3))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 8 bytes
			case EqualLong:
				thread->memory->stackPointer-=15;
				if(*(long*)(thread->memory->stack + (thread->memory->stackPointer-1)) == 
					*(long*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 4 bytes
			case EqualFloat:
				thread->memory->stackPointer-=7;

				if((*(float*)(thread->memory->stack + (thread->memory->stackPointer-1))) == 
					(*(float*)(thread->memory->stack + thread->memory->stackPointer+3)))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;

			//Compare value of 2 8 bytes
			case EqualDouble:
				thread->memory->stackPointer-=15;
				if(*(double*)(thread->memory->stack + (thread->memory->stackPointer-1)) ==
					*(double*)(thread->memory->stack + thread->memory->stackPointer+7))
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 1;
				}
				else
				{
					thread->memory->stack[thread->memory->stackPointer-1] = 0;
				}
				break;


			case If:

				if(*(char*)(thread->memory->stack + --thread->memory->stackPointer))
				{
					thread->programCounter++;
					thread->programCounter = 
					*((unsigned long*)(thread->bytecode + (thread->programCounter)));
				}

				else
				{
					thread->programCounter += 7;
				}
				break;


			case IfNot:
				if(!(*(char*)(thread->memory->stack + (--thread->memory->stackPointer))))
				{
					thread->programCounter++;
					thread->programCounter = 
					*((unsigned long*)(thread->bytecode + (thread->programCounter)));
				}

				else
				{
					thread->programCounter += 7;
				}
				break;	
		}
		thread->programCounter++;
	}
}