#ifndef SPUMETHODS_H
#define SPUMETHODS_H

#include <stdio.h>

#include "Stack.h"
#include "Default.h"

//todo pointers at arrays in SPU struct and calloc

struct SPU {
    int ip = 0;
    Stack* st = NULL;
    Stack* stFunc = NULL;
    int cmds[CMDS_SIZE] = {};
    int regs[REGS_SIZE] = {};
    int ram[RAM_SIZE] = {};
};

SPU* SpuInit();

void SpuDtor(SPU* spu);

#endif // SPUMETHODS_H