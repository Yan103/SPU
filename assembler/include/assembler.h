#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "/home/yan/projects/processor/cpu/include/Default.h"
#include "/home/yan/projects/processor/cpu/include/ReturnCodes.h"

static const char* INPUT_FILENAME  = "/home/yan/projects/processor/program.asm";
static const char* OUTPUT_FILENAME = "/home/yan/projects/processor/assembler_code.txt";

Reg GetRegValue(char* cmd);

void SkipAsmComments(char* curr_line);
void GetCommandsArgs(int argc,  char* argv[]);
void FillArgType(char* arg, int* arg_type);
void PushPopFill(int* cmds, int* ip, char* arg);
char* SearchConstPtr(char* str, size_t str_len);

void FromTextToCode(char* cmd, int* machine_code);

void Assembler(FILE* input_file, FILE* output_file);

#endif // ASSEMBLER_H