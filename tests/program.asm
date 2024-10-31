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
NZA:
        push 11
        out
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
        push 666666666
        out
        hlt