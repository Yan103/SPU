#include <stdio.h>

#include "Default.h"
#include "ReturnCodes.h"
#include "StackMethods.h"
#include "Stack.h"
#include "Config.h"
#include "SpuMethods.h"

#include <stdlib.h>

SPU* SpuInit() {
    SPU* spu = (SPU*)calloc(1, sizeof(SPU));
    ASSERT(spu != NULL, "MEMORY ERROR!\n");

    spu->st = STACK_INIT(spu->st);
    spu->stFunc = STACK_INIT(spu->stFunc);

    return spu;
}

void SpuDtor(SPU* spu) {
    ASSERT(spu != NULL, "NULL POINTER!\n");

    StackDtor(spu->st);
    StackDtor(spu->stFunc);

    FREE(spu)
}