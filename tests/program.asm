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
pop DX
push 0
push AX
jne NZA
je ZA
DZ:
        push 0
        push BX
        sub
        push 2
        push AX
        mul
        div
        out
        hlt
DGZ:
        push 0
        push BX
        sub
        push DX
        sqrt
        add
        push 2
        push AX
        mul
        div
        out; 1 root
        push 0
        push BX
        sub
        push DX
        sqrt
        sub
        push 2
        push AX
        mul
        div
        out; 2 root
        hlt
ZA:
        push 0
        push BX
        jne NZB
        je ZB
NZB:
        push 0
        push CX
        sub
        push BX
        div
        out
        hlt
ZB:
        push 0
        push CX
        je ZC
        jne NOROOTS
ZC:
        push 888
        out
        hlt
NOROOTS:
        push 6666
        out
        hlt
NZA:
        push DX
        jlz NOROOTS
        jgz DGZ
        push 0
        je DZ