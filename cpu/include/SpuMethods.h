#ifndef SPUMETHODS_H
#define SPUMETHODS_H

#include <stdio.h>

#include "Stack.h"
#include "Default.h"

struct SPU {
    Stack* st = NULL;
    int cmds[CMDS_SIZE] = {};
    int regs[REGS_SIZE] = {};
};

SPU* SpuInit();

void SpuDtor(SPU* spu);

#endif // SPUMETHODS_H