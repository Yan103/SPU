#ifndef RETURNCODES_H
#define RETURNCODES_H

#include <stdio.h>

#include "Default.h"
#include "Stack.h"

static const size_t LABEL_NAME_LEN = 10;
static const size_t LABELS_COUNT   = 10;

/// @brief Enum with return functions codes
enum FuncReturn {
    SUCCESS        = 0b0,        ///< SUCCESS
    BAD_PTR        = 0b1,        ///< BAD POINTER
    BAD_SIZE       = 0b10,       ///< BAD STACK SIZE
    BAD_FILE       = 0b100,      ///< BAD FILENAME
    EMPTY_STACK    = 0b1000,     ///< EMPTY STACK
    STACK_OVERFLOW = 0b10000,    ///< STACK OVERFLOW
    KANARY_DAMAGED = 0b100000,   ///< KANARY DAMAGED
    HASH_ERROR     = 0b1000000,  ///< HASH ERROR
    MEMORY_ERROR   = 0b10000000, ///< MEMORY ERROR
    ERROR          = -1,
};

enum Comands {
    PUSH  =  1,
    POP   =  2,
    LABEL =  3,
    ADD   =  4,
    SUB   =  5,
    DIV   =  6,
    MUL   =  7,
    IN    =  8,
    OUT   =  9,
    JNE   = 10,
    JMP   = 11,
    JZ    = 12,
    JGZ   = 13,
    JLZ   = 14,
    CALL  = 15,
    RET   = 17,
    HLT   = -1,
};

enum Reg {
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    EX = 4,
    XX = 5,
};

struct Label {
    char label_name[LABEL_NAME_LEN];
    int  label_number = -1;
};

#endif // RETURNCODES_H