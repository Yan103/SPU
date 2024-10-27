#include <stdio.h>
#include <string.h>

#include "Default.h"
#include "ReturnCodes.h"
#include "StackMethods.h"
#include "Stack.h"
#include "Config.h"
#include "SpuMethods.h"

// stupid comment

static void GetCommandsArgs(int argc,  char* argv[]);

static const char* INPUT_FILENAME  = "/home/yan/projects/processor/assembler_code.txt";

static void GetCommandsArgs(int argc,  char* argv[]) {
    assert(argv != NULL);

    for (int i = 1; i < argc; i++) {
        if (strcasecmp(argv[i], "-f") == 0) {
            if (i != argc - 1)
                INPUT_FILENAME = argv[i + 1];
        }
    }
}

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

    int DoFlag = 1;

    while (DoFlag) {
        switch(spu->cmds[ spu->ip ]) {
            case PUSH: {
                StackPush(spu->st, spu->cmds[ ++(spu->ip) ]);
                (spu->ip)++;
                break;
            }

            case PUSHR: {
                spu->regs[ spu->cmds[ ++(spu->ip) ] ] = StackPop(spu->st);
                (spu->ip)++;
                break;
            }

            case POPR: {
                StackPush(spu->st, spu->regs[ spu->cmds[ ++(spu->ip) ] ]);
                (spu->ip)++;
                break;
            }

            case ADD: {
                StackPush(spu->st, StackPop(spu->st) + StackPop(spu->st));
                (spu->ip)++;
                break;
            }

            case CPY: {
                int a = StackPop(spu->st);
                StackPush(spu->st, a);
                StackPush(spu->st, a);
                (spu->ip)++;
                break;
            }

            case SUB: {
                int a = StackPop(spu->st);
                StackPush(spu->st, StackPop(spu->st) - a);
                (spu->ip)++;
                break;
            }

            case DIV: {
                int a = StackPop(spu->st);
                if (a != 0)
                    StackPush(spu->st, StackPop(spu->st) / a);
                else {
                    printf(RED("ERROR! DIVISION BY ZERO!!!\n"));
                    DoFlag = 0;
                }
                (spu->ip)++;
                break;
            }

            case MLT: {
                StackPush(spu->st, StackPop(spu->st) * StackPop(spu->st));
                (spu->ip)++;
                break;
            }

            case IN: {
                int a = 0;
                printf(YELLOW("Enter a number: ")); scanf("%d", &a);
                StackPush(spu->st, a);
                (spu->ip)++;
                break;
            }

            case OUT: {
                printf(GREEN("RESULT: %d\n"), StackPop(spu->st));
                (spu->ip)++;
                break;
            }

            case HLT: {
                DoFlag = 0;
                break;
            }

            case JMP: {
                int next_ip = spu->cmds[ ++(spu->ip) ];
                spu->ip = next_ip;
                break;
            }

            case CALL: {
                StackPush(spu->stFunc, spu->ip + 2);
                int next_ip = spu->cmds[ ++(spu->ip) ];
                spu->ip = next_ip;
                break;
            }

            case RET: {
                int next_ip = StackPop(spu->stFunc);
                spu->ip = next_ip;
                break;
            }

            case JNE: {
                int a = StackPop(spu->st);
                int b = StackPop(spu->st);

                if (a != b) {
                    int next_ip = spu->cmds[ ++(spu->ip) ];
                    spu->ip = next_ip;
                } else spu->ip += 2;

                StackPush(spu->st, b);
                StackPush(spu->st, a);
                break;
            }

            default: {
                printf("%d %d", spu->ip, spu->cmds[ spu->ip ]);
                printf(RED("UNKNOWN CODE!!!\n"));
                break;
            }
        }
    }

    SpuDtor(spu);

    return SUCCESS;
}