#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <stdlib.h>

#include "assembler.h"
#include "/home/yan/projects/processor/cpu/include/ReturnCodes.h"

char* SearchConstPtr(char* str, size_t str_len) {
    assert(str != NULL);

    for (size_t i = 0; i < str_len; i++) {
        if (isdigit(*(str++))) return --str;
    }

    return NULL;
}

//! -lm

void FillArgType(char* arg, int* arg_type) {
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
    } else if (reg_ptr != NULL) {
        *arg_type |= 1;
    } else {
        *arg_type |= 2;
    }
}

Reg GetRegValue(char* cmd) {
    assert(cmd != NULL);

         if (strstr(cmd, "AX")) return AX;
    else if (strstr(cmd, "BX")) return BX;
    else if (strstr(cmd, "CX")) return CX;
    else if (strstr(cmd, "DX")) return DX;

    return AX;
}

void SkipAsmComments(char* curr_line) {
    assert(curr_line != NULL);

    for (size_t i = 0; i < strlen(curr_line); i++) {
        if (curr_line[i] == ';') {
            curr_line[i] = '\0';
            break;
        }
    }
}

void GetCommandsArgs(int argc,  char* argv[]) {
    assert(argc != 0);
    assert(argv != NULL);

    for (int i = 1; i < argc; i++) {
        if (strcasecmp(argv[i], "-f") == 0) {
            if (i != argc - 1) {
                INPUT_FILENAME = argv[i + 1];
            }
        } else if (strcasecmp(argv[i], "-o") == 0) {
            if (i != argc - 1)
                OUTPUT_FILENAME = argv[i + 1];
        }
    }
}

void FromTextToCode (char* cmd, int* machine_cmd) {
    if      (strchr(cmd, ':')         != 0)  *machine_cmd = LABEL;
    else if (strcasecmp(cmd, "push")  == 0)  *machine_cmd = PUSH;
    else if (strcasecmp(cmd, "pop")   == 0)  *machine_cmd = POP;
    else if (strcasecmp(cmd, "add")   == 0)  *machine_cmd = ADD;
    else if (strcasecmp(cmd, "sub")   == 0)  *machine_cmd = SUB;
    else if (strcasecmp(cmd, "mul")   == 0)  *machine_cmd = MUL;
    else if (strcasecmp(cmd, "div")   == 0)  *machine_cmd = DIV;
    else if (strcasecmp(cmd, "in")    == 0)  *machine_cmd = IN;
    else if (strcasecmp(cmd, "out")   == 0)  *machine_cmd = OUT;
    else if (strcasecmp(cmd, "jmp")   == 0)  *machine_cmd = JMP;
    else if (strcasecmp(cmd, "jne")   == 0)  *machine_cmd = JNE;
    else if (strcasecmp(cmd, "call")  == 0)  *machine_cmd = CALL;
    else if (strcasecmp(cmd, "ret")   == 0)  *machine_cmd = RET;
    else if (strcasecmp(cmd, "hlt")   == 0)  *machine_cmd = HLT;
    // TODO error check
}

void PushPopFill(int* cmds, int* ip, char* arg) {
    assert(cmds != NULL);
    assert(ip   != NULL);
    assert(arg  != NULL);

    int arg_type = 0;
    FillArgType(arg, &arg_type);

    cmds[ (*ip)++ ] = arg_type;

    if (arg_type & 1) {
        Reg tmp = GetRegValue(arg);
        cmds[ (*ip)++ ] = tmp;
    }

    if (arg_type & 2) {
        char* const_ptr = SearchConstPtr(arg, strlen(arg)); // TODO check for NULL!
        if (const_ptr != NULL) {
            sscanf(const_ptr, "%d", &cmds[(*ip)++]);
        }
    }
}

void FormateArg (char* push_arg_unformated, char* push_arg) {
    assert (push_arg_unformated != NULL);

    char symb = *push_arg_unformated;
    char push_arg_formated[20] = {};
    int format_ptr = 0;

    while (symb == ' ') {
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
    }

    for (int i = 0; i < 20 - 1; i++) {
        if (symb != '\r') {
            push_arg_formated[format_ptr] = symb;
            format_ptr += 1;
        }
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
    }

    memcpy(push_arg, push_arg_formated, 20);
}

void Assembler(FILE* input_filename, FILE* output_filename) {
    assert(input_filename  != NULL);
    assert(output_filename != NULL);

    char cmd[20] = {};
    char curr_line[MAXLINE] = {};
    int  cmds[CMDS_SIZE] = {};
    int i = 0, Doflag = 1;

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
        int machine_code = -2;
        FromTextToCode(cmd, &machine_code);

        switch (machine_code) {
            case PUSH:
            case POP: {
                cmds[i++] = machine_code;
                sscanf(curr_line, "%s %[^\n]", cmd, cmd);
                char arg[20] = {};
                FormateArg(cmd, arg);
                PushPopFill(cmds, &i, arg);

                continue;
            }

            case ADD:
            case SUB:
            case DIV:
            case MUL:
            case IN:
            case OUT:
            case RET:
            case HLT: {
                cmds[i++] = machine_code;


                continue;
            }

            case JMP:
            case JNE:
            case CALL: {
                cmds[i++] = machine_code;
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

            case LABEL: {
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

            default: printf(RED("UNDEFINED COMMAND!!!\n"));
        }

    }

    fwrite(cmds, sizeof(cmds[0]), (size_t)i, output_filename);
}