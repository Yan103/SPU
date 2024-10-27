#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <stdlib.h>

#include "/home/yan/projects/processor/cpu/include/Default.h"
#include "/home/yan/projects/processor/cpu/include/ReturnCodes.h"

static Reg GetRegValue(char* cmd);
static void SkipAsmComments(char* curr_line);
static void GetCommandsArgs(int argc,  char* argv[]);
static void FillArgType(char* arg, int* arg_type);
static void PushPopFill(int* cmds, int* ip, char* arg);
static char* SearchConstPtr(char* str, size_t str_len);

static const char* INPUT_FILENAME  = "/home/yan/projects/processor/program.asm";
static const char* OUTPUT_FILENAME = "/home/yan/projects/processor/assembler_code.txt";

static char* SearchConstPtr(char* str, size_t str_len) {
    assert(str != NULL);

    for (size_t i = 0; i < str_len; i++) {
        if (isdigit(*(str++))) return --str;
    }

    return NULL;
}

static void FillArgType(char* arg, int* arg_type) {
    assert(arg != NULL);
    assert(arg_type != NULL);

    char* open_bracket = strchr(arg, '[');
    char* plus_ptr     = strchr(arg, '+');
    char* reg_ptr      = strchr(arg, 'X');

    if (open_bracket != NULL) {
        //if (strchr(arg, ']') == NULL) return ERROR; // TODO smt with it // TODO! error enum for cpu/asm
        *arg_type |= 4;
    }

    if (plus_ptr != NULL) {
        *arg_type |= 3;
    }
    else if (reg_ptr != NULL) {
        *arg_type |= 1;
    }
    else {
        *arg_type |= 2;
    }
}

static Reg GetRegValue(char* cmd) {
    assert(cmd != NULL);

    if (strcasecmp(cmd, "ax") == 0) return AX;
    if (strcasecmp(cmd, "bx") == 0) return BX;
    if (strcasecmp(cmd, "cx") == 0) return CX;
    if (strcasecmp(cmd, "dx") == 0) return DX;
    return EX;
}

static void SkipAsmComments(char* curr_line) {
    assert(curr_line != NULL);

    for (size_t i = 0; i < strlen(curr_line); i++) {
        if (curr_line[i] == ';') {
            curr_line[i] = '\0';
            break;
        }
    }
}

static void GetCommandsArgs(int argc,  char* argv[]) {
    assert(argv != NULL);

    for (int i = 1; i < argc; i++) {
        if (strcasecmp(argv[i], "-f") == 0) {
            if (i != argc - 1)
                INPUT_FILENAME = argv[i + 1];
        } else if (strcasecmp(argv[i], "-o") == 0) {
            if (i != argc - 1)
                OUTPUT_FILENAME = argv[i + 1];
        }
    }
}

static void PushPopFill(int* cmds, int* ip, char* arg) {
    assert(cmds != NULL);
    assert(ip   != NULL);
    assert(arg  != NULL);

    int arg_type = 0;
    FillArgType(arg, &arg_type);

    cmds[ (*ip)++ ] = arg_type;

    if (arg_type & 1) {
        cmds[(*ip)++] = GetRegValue(arg);
    }

    if (arg_type & 2) {
        char* const_ptr = SearchConstPtr(arg, strlen(arg)); // TODO check for NULL!
        if (const_ptr != NULL) {
            sscanf(const_ptr, "%d", &cmds[(*ip)++]);
        }
    }
}

int main(int argc, char* argv[]) {
    GetCommandsArgs(argc, argv);

    FILE* input_filename = fopen(INPUT_FILENAME, "r");

    if (!input_filename) {
        printf(RED("Error occured while opening file\n"));

        return BAD_FILE;
    }

    char cmd[10] = {};
    char curr_line[MAXLINE] = {};
    int  cmds[CMDS_SIZE] = {};
    int  arg = 0, i = 0, Doflag = 1;

    Label Labels[LABELS_COUNT] = {};
    char* fflag = {};

    while ((fflag = fgets(curr_line, MAXLINE, input_filename)) != NULL || Doflag >= 0) {
        SkipAsmComments(curr_line);

        if (fflag == NULL) {
            i = 0;
            Doflag--;
            fseek(input_filename, 0L, SEEK_SET);
            continue;
        }

        sscanf(curr_line, "%s", cmd);

        if (strcasecmp(cmd, "push") == 0) {
            cmds[i++] = PUSH;
            sscanf(curr_line, "%s %s", cmd, cmd);
            PushPopFill(cmds, &i, cmd);
            continue;
        }
        if (strcasecmp(cmd, "pop") == 0) {
            cmds[i++] = POP;
            sscanf(curr_line, "%s %s", cmd, cmd);
            PushPopFill(cmds, &i, cmd);
            continue;
        }
        /*if (strcasecmp(cmd, "pushr") == 0)  {
            cmds[i++] = PUSHR;
            sscanf(curr_line, "%s %s", cmd, cmd);
            cmds[i++] = GetRegValue(cmd);
            continue;
        }
        if (strcasecmp(cmd, "popr") == 0) {
            cmds[i++] = POPR;
            sscanf(curr_line, "%s %s", cmd, cmd);
            cmds[i++] = GetRegValue(cmd);
            continue;
        }*/
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
        if (strcasecmp(cmd, "ret") == 0) {
            cmds[i++] = RET;
            continue;
        }
        if (strcasecmp(cmd, "cpy") == 0) {
            cmds[i++] = CPY;
            continue;
        }
        if (strcasecmp(cmd, "hlt") == 0) {
            cmds[i++] = HLT;
            Doflag--;
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
            sscanf(curr_line, "%s %s", cmd, cmd);

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
            sscanf(curr_line, "%s %s", cmd, cmd);

            for (size_t j = 0; j < LABELS_COUNT; j++) {
                if (strcasecmp(Labels[j].label_name, cmd) == 0) {
                    cmds[i++] = Labels[j].label_number;
                    break;
                }
            }
            continue;
        }
        if (strcasecmp(cmd, "call") == 0) {
            cmds[i++] = CALL;
            sscanf(curr_line, "%s %s", cmd, cmd);

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