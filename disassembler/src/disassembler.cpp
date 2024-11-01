#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include "/home/projects/SPU/cpu/include/Default.h"
#include "/home/projects/SPU/cpu/include/ReturnCodes.h"

static const char* INPUT_FILENAME  = "/home/projects/SPU/assembler_code.txt";
static const char* OUTPUT_FILENAME = "/home/projects/SPU/disassembler_code.txt";


int main() {
    FILE* input_filename = fopen(INPUT_FILENAME, "rb");

    if (!input_filename) {
        printf(RED("Error occured while opening file\n"));

        return BAD_FILE;
    }

    int cmds[CMDS_SIZE] = {};

    fread(cmds, sizeof(int), CMDS_SIZE, input_filename);
    fclose(input_filename);

    FILE* output_filename = fopen(OUTPUT_FILENAME, "w");

    if (!output_filename) {
        printf(RED("Error occured while opening file\n"));

        return BAD_FILE;
    }

    int i = 0, DoFlag = 1;

    while (i != CMDS_SIZE && DoFlag == 1) {
        switch (cmds[i]) {
            case PUSH:
                fprintf(output_filename, "%s ", "push");
                fprintf(output_filename, "%d\n", cmds[i+1]);
                i += 2;
                break;

            case PUSHR:
                fprintf(output_filename, "%s ", "pushr");
                fprintf(output_filename, "%d\n", cmds[i+1]);
                i += 2;
                break;

            case POPR:
                fprintf(output_filename, "%s\n", "popr");
                i++;
                break;

            case ADD:
                fprintf(output_filename, "%s\n", "add");
                i++;
                break;

            case SUB:
                fprintf(output_filename, "%s\n", "sub");
                i++;
                break;

            case DIV:
                fprintf(output_filename, "%s\n", "div");
                i++;
                break;

            case MLT:
                fprintf(output_filename, "%s\n", "mlt");
                i++;
                break;

            case IN:
                fprintf(output_filename, "%s\n", "in");
                i++;
                break;

            case OUT:
                fprintf(output_filename, "%s\n", "out");
                i++;
                break;

            case JMP:
                fprintf(output_filename, "%s ", "jmp");
                fprintf(output_filename, "%d", cmds[i++]);
                fprintf(output_filename, "%s\n", ":");
                i++;
                break;

            case JNE:
                fprintf(output_filename, "%s\n", "jne");
                fprintf(output_filename, "%d", cmds[i++]);
                fprintf(output_filename, "%s\n", ":");
                i++;
                break;

            case HLT:
                fprintf(output_filename, "%s", "hlt");
                DoFlag = 0;
                break;

            default:
                fprintf(output_filename, "%d", cmds[i++]);
                fprintf(output_filename, "%s\n", ":");
                break;
            }
    }

    fclose(output_filename);

    printf(GREEN("DISASSEMBLING END!\n"));

    return SUCCESS;
}