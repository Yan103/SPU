/*!
    \file
    File with the declaration of assembler's functions
*/

#include <cassert>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "/home/yan/projects/processor/cpu/include/ReturnCodes.h"

/// @brief Constant for the register bit
const int REG_BIT           = 1;

/// @brief Constant for the memory bit
const int MEMORY_BIT        = 4;

/// @brief Constant for the constant bit
const int CONSTANT_BIT      = 2;

/// @brief Constant for the maximum length of the command
const size_t MAX_CMD_LENGTH = 50;

/*!
    @brief Function that search a constant in command argument
    \param [out] str     - argument
    \param [in]  str_len - length of the argument
*/
char* SearchConstPtr(char* str, size_t str_len) {
    assert(str != NULL);

    for (size_t i = 0; i < str_len; i++) {
        if (isdigit(*(str++))) return --str;
    }

    return NULL;
}

/*!
    @brief Function that fill the argument type with bits if necessary
    \param [in]  arg      - command argument
    \param [out] arg_type - type of argument
*/
FuncReturn FillArgType(char* arg, int* arg_type) {
    assert(arg != NULL);
    assert(arg_type != NULL);

    char* open_bracket = strchr(arg, '[');
    char* plus_ptr     = strchr(arg, '+');
    char* reg_ptr      = strchr(arg, 'X');

    if (open_bracket != NULL) {
        if (strchr(arg, ']') == NULL) {
            return ERROR;
        }
        *arg_type |= MEMORY_BIT;
    }

    if (plus_ptr != NULL) {
        *arg_type |= REG_BIT;
        *arg_type |= CONSTANT_BIT;
    } else if (reg_ptr != NULL) {
        *arg_type |= REG_BIT;
    } else {
        *arg_type |= CONSTANT_BIT;
    }

    return SUCCESS;
}

#define DEF_REGISTER_(REGISTER_NAME) {                 \
    if (strstr(cmd, #REGISTER_NAME))                   \
        return REGISTER_NAME;                          \
}

/*!
    @brief Function that get register value from argument of command
    \param [in] cmd - command
*/
Reg GetRegValue(char* cmd) {
    assert(cmd != NULL);

    //*IMPORTANT
    //*Automatic code generation is used here.
    //*Please, if you want to change something in the code,
    //*do everything carefully through the appropriate file or remove the code generation altogether

    #include "registrs.txt"

    return XX;
}
#undef DEF_REGISTER_

/*!
    @brief Function that skip comments in assembler code
    \param [out] curr_line - current line
*/
FuncReturn SkipAsmComments(char* curr_line) {
    assert(curr_line != NULL);

    for (size_t i = 0; i < strlen(curr_line); i++) {
        if (curr_line[i] == ';') {
            curr_line[i] = '\0';
            break;
        }
    }

    return SUCCESS;
}

/*!
    @brief Function that get arguments from the command line
    \param [in] argc - argument count
    \param [in] argv - argument values
*/
FuncReturn GetCommandsArgs(int argc,  char* argv[]) {
    assert(argc != 0);
    assert(argv != NULL);

    for (int i = 1; i < argc; i++) {
        if (strcasecmp(argv[i], "-f") == 0) {
            if (i != argc - 1) {
                INPUT_FILENAME = argv[i + 1];
            }
        } else if (strcasecmp(argv[i], "-o") == 0) {
            if (i != argc - 1) {
                OUTPUT_FILENAME = argv[i + 1];
            }
        }
    }

    return SUCCESS;
}

#define DEF_COMMAND_(COMMAND_NAME) {                                       \
    if (strcasecmp(cmd, #COMMAND_NAME) == 0) *machine_cmd  = COMMAND_NAME; \
}

/*!
    @brief Function that formats the text command to code type
    \param [in]  cmd          - command
    \param [out] machine_code - future formatted machine code
*/
void FromTextToCode (char* cmd, int* machine_cmd) {
    if (strchr(cmd, ':') != 0)  *machine_cmd  = LABEL;

    //*IMPORTANT
    //*Automatic code generation is used here.
    //*Please, if you want to change something in the code,
    //*do everything carefully through the appropriate file or remove the code generation altogether

    #include "commands.txt"
}
#undef DEF_COMMAND_

/*!
    @brief Function that that generates arguments for Push and Pop
    \param [in]  cmds - array with commands
    \param [out] ip   - instruction pointer
    \param [in]  arg  - argument
*/
FuncReturn PushPopFill(int* cmds, int* ip, char* arg) {
    assert(cmds != NULL);
    assert(ip   != NULL);
    assert(arg  != NULL);

    int arg_type = 0;
    FillArgType(arg, &arg_type);

    cmds[ (*ip)++ ] = arg_type;

    if (arg_type & REG_BIT) {
        Reg reg_value = GetRegValue(arg);
        cmds[ (*ip)++ ] = reg_value;
    }

    if (arg_type & CONSTANT_BIT) {
        char* const_ptr = SearchConstPtr(arg, strlen(arg));
        if (const_ptr != NULL) {
            sscanf(const_ptr, "%d", &cmds[ (*ip)++ ]);
        } else {
            return ERROR;
        }
    }

    return SUCCESS;
}

/*!
    @brief Function that formats the received argument
    \param [in] push_arg_unformated - unformatted argument
    \param [in] push_arg            - future formatted argument
*/
FuncReturn FormateArg (char* push_arg_unformated, char* push_arg) {
    assert (push_arg_unformated != NULL);

    char symb = *push_arg_unformated;
    char push_arg_formated[MAX_CMD_LENGTH] = {};
    int format_ptr = 0;

    while (symb == ' ') {
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
    }

    for (size_t i = 0; i < MAX_CMD_LENGTH - 1; i++) {
        if (symb != '\r') {
            push_arg_formated[format_ptr] = symb;
            format_ptr += 1;
        }
        push_arg_unformated += 1;
        symb = *push_arg_unformated;
    }

    memcpy(push_arg, push_arg_formated, MAX_CMD_LENGTH);

    return SUCCESS;
}

/*!
    @brief Assembler function
    \param [in] input_file  - input filename
    \param [in] output_file - output filename
*/
void Assembler(FILE* input_filename, FILE* output_filename) {
    assert(input_filename  != NULL);
    assert(output_filename != NULL);

    char cmd[MAX_CMD_LENGTH] = {};
    char curr_line[MAXLINE] = {};
    int  cmds[CMDS_SIZE] = {};

    int i = 0, Doflag = 1;
    int machine_code = 0;

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
        FromTextToCode(cmd, &machine_code);

        switch (machine_code) {
            case PUSH:
            case POP: {
                cmds[i++] = machine_code;
                sscanf(curr_line, "%s %[^\n]", cmd, cmd);
                char arg[MAX_CMD_LENGTH] = {};
                FormateArg(cmd, arg);
                PushPopFill(cmds, &i, arg);

                continue;
            }

            case ADD:
            case SQRT:
            case SQR:
            case MOD:
            case IDIV:
            case SUB:
            case DIV:
            case MUL:
            case IN:
            case OUT:
            case RET:
            case DRAW:
            case HLT: {
                cmds[i++] = machine_code;

                continue;
            }

            case JMP:
            case JNE:
            case JE:
            case JLZ:
            case JGZ:
            case JZ:
            case JGE:
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