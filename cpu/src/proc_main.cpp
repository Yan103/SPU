/*!
    \file
    File with processor main function
*/

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

    for (size_t i = 0; i < 200; i++) {
        printf("%c ", spu->ram[i]);
        if (i + 1 % 50 == 0) printf("\n");
    }


    SpuDtor(spu);

    return SUCCESS;
}