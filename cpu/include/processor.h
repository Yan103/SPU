#include <stdio.h>

#include "SpuMethods.h"

static const char* INPUT_FILENAME  = "/home/yan/projects/processor/assembler_code.txt";

void GetCommandsArgs(int argc,  char* argv[]);

void CPUWork(SPU* spu);

int* GetArg (SPU* spu);
