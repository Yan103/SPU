#include <stdio.h>
#include <string.h>
#include <cassert>

#include "/home/projects/SPU/cpu/include/Default.h"
#include "/home/projects/SPU/cpu/include/ReturnCodes.h"

static const char* INPUT_FILENAME  = "/home/projects/SPU/program.asm";
static const char* OUTPUT_FILENAME = "/home/projects/SPU/assembler_code.txt";

static Reg GetRegValue(char* cmd);

static Reg GetRegValue(char* cmd) {
    assert(cmd != NULL);

    if (strcasecmp(cmd, "ax") == 0) return AX;
    if (strcasecmp(cmd, "bx") == 0) return BX;
    if (strcasecmp(cmd, "cx") == 0) return CX;
    if (strcasecmp(cmd, "dx") == 0) return DX;
    return EX;
}

int main() {
    FILE* input_filename = fopen(INPUT_FILENAME, "r");

    if (!input_filename) {
        printf(RED("Error occured while opening file\n"));

        return BAD_FILE;
    }

    char cmd[10] = {};
    int  cmds[CMDS_SIZE] = {};
    int  arg = 0, i = 0, Doflag = 1;

    Label Labels[LABELS_COUNT] = {};

    while (fscanf(input_filename, "%s", cmd) != EOF && Doflag >= 0) {
        if (strcasecmp(cmd, "push") == 0) {
            fscanf(input_filename, "%d", &arg);
            cmds[i++] = PUSH;
            cmds[i++] = arg;
            continue;
        }
        if (strcasecmp(cmd, "pushr") == 0)  {
            cmds[i++] = PUSHR;
            fscanf(input_filename, "%s", cmd);
            cmds[i++] = GetRegValue(cmd);
            continue;
        }
        if (strcasecmp(cmd, "popr") == 0) {
            cmds[i++] = POPR;
            fscanf(input_filename, "%s", cmd);
            cmds[i++] = GetRegValue(cmd);
            continue;
        }
        if (strcasecmp(cmd, "add") == 0) {
            cmds[i++] = ADD;
            continue;
        }
        if (strcasecmp(cmd, "sub") == 0) {
            cmds[i++] = SUB;
            continue;
        }
        if (strcasecmp(cmd, "div") == 0) {
            cmds[i++] = DIV;
            continue;
        }
        if (strcasecmp(cmd, "mlt") == 0) {
            cmds[i++] = MLT;
            continue;
        }
        if (strcasecmp(cmd, "in") == 0) {
            cmds[i++] = IN;
            continue;
        }
        if (strcasecmp(cmd, "out") == 0) {
            cmds[i++] = OUT;
            continue;
        }
        if (strcasecmp(cmd, "hlt") == 0) {
            cmds[i++] = HLT;
            Doflag--;
            i = 0;
            fseek(input_filename, 0L, SEEK_SET);
            continue;
        }
        if (strstr(cmd, ":")) {
            for (size_t j = 0; j < LABELS_COUNT; j++) {
                if (Labels[j].label_number == -1) {
                    Labels[j].label_number = i;
                    cmd[ strlen(cmd) - 1 ] = '\0';
                    strcpy(Labels[j].label_name, cmd);
                    break;
                }
            }
            continue;
        }
        if (strcasecmp(cmd, "jmp") == 0) {
            cmds[i++] = JMP;
            fscanf(input_filename, "%s", cmd);
            if (Doflag == 0) cmd[ strlen(cmd) - 1] = '\0';

            for (size_t j = 0; j < LABELS_COUNT; j++) {
                if (strcasecmp(Labels[j].label_name, cmd) == 0) {
                    cmds[i++] = Labels[j].label_number;
                    break;
                } else if (j == LABELS_COUNT - 1) {
                    cmds[i++] = -1;
                }
            }
            continue;
        }
        if (strcasecmp(cmd, "jne") == 0) {
            cmds[i++] = JNE;
            fscanf(input_filename, "%s", cmd);

            for (size_t j = 0; j < LABELS_COUNT; j++) {
                if (strcasecmp(Labels[j].label_name, cmd) == 0) {
                    cmds[i++] = Labels[j].label_number;
                    break;
                }
            }
            continue;
        }

        printf(RED("UNDEFINED COMMAND!!!\n"));
    }

    FILE* output_filename = fopen(OUTPUT_FILENAME, "wb");

    if (!output_filename) {
        printf(RED("Error occured while opening file\n"));

        return BAD_FILE;
    }

    fwrite(cmds, sizeof(cmds[0]), sizeof(cmds) / sizeof(cmds[0]), output_filename);

    fclose(input_filename);
    fclose(output_filename);

    printf(GREEN("ASSEMBLING END!\n"));

    return SUCCESS;
}