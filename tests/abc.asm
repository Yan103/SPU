in
in
in
pop CX
pop BX
pop AX
push BX
push BX
mul
push 4
push AX
push CX
mul
mul
sub
out
hlt
pop DX
push 0
push AX
jne ZA
je NZA
ZA:
        push 0
        push BX
        jne NZB
        je ZB
ZB:
        push 0
        push CX
        je INFROOTS
        jne NOROOTS
INFROOTS:
        push -1
        out
        hlt
NZB:
        push -1
        push CX
        mul
        out
        hlt
NZA:
        push DX
        jlz NOROOTS
        jz 1ROOT
        jgz 2ROOTS
NOROOTS:
        push 0
        out
        hlt
1ROOT:
        push 1
        out
        hlt
2ROOTS:
        push 2
        out
        hlt