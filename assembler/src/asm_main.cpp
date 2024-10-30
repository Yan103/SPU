#include <stdio.h>
#include <string.h>

#include "assembler.h"

int main(int argc, char* argv[]) {
    GetCommandsArgs(argc, argv);

    FILE* input_filename = fopen(INPUT_FILENAME, "r");

    if (!input_filename) {
        printf(RED("Error occured while opening input file!\n"));

        return BAD_FILE;
    }

    FILE* output_filename = fopen(OUTPUT_FILENAME, "wb");

    if (!output_filename) {
        printf(RED("Error occured while opening output file!\n"));

        return BAD_FILE;
    }

    Assembler(input_filename, output_filename);

    fclose(input_filename);
    fclose(output_filename);

    printf(GREEN("ASSEMBLING END!\n"));

    return SUCCESS;
}