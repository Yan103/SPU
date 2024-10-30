
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "/home/yan/projects/processor/cpu/include/Default.h"
#include "/home/yan/projects/processor/cpu/include/ReturnCodes.h"

static const char* INPUT_FILENAME  = "/home/yan/projects/processor/tests/program.asm";
static const char* OUTPUT_FILENAME = "/home/yan/projects/processor/tests/assembler_code.txt";

Reg GetRegValue(char* cmd);

FuncReturn SkipAsmComments(char* curr_line);

FuncReturn GetCommandsArgs(int argc,  char* argv[]);

FuncReturn FillArgType(char* arg, int* arg_type);

FuncReturn PushPopFill(int* cmds, int* ip, char* arg);

char* SearchConstPtr(char* str, size_t str_len);

void FromTextToCode(char* cmd, int* machine_code);

void Assembler(FILE* input_file, FILE* output_file);

FuncReturn FormateArg(char* push_arg_unformated, char* push_arg);

#endif // ASSEMBLER_H
