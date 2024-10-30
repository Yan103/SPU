#include <stdio.h>
#include <string.h>

#include "Default.h"
#include "ReturnCodes.h"
#include "StackMethods.h"
#include "Stack.h"
#include "Config.h"
#include "SpuMethods.h"
#include "processor.h"

int main (int argc, char* argv[]) {
    GetCommandsArgs(argc, argv);

    SPU* spu = SpuInit();

    FILE* input_file = fopen(INPUT_FILENAME, "rb");

    if (!input_file) {
        printf(RED("Error occured while opening file\n"));

        return BAD_FILE;
    }

    fread(spu->cmds, sizeof(StackElem_t), CMDS_SIZE, input_file);

    fclose(input_file);

    CPUWork(spu);

    printf("%d\n", spu->regs[0]);
    printf("%d\n", spu->ram[ 5 + 5 ]);

    SpuDtor(spu);

    return SUCCESS;
}