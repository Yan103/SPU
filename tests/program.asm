in; radius
in; x_centr
in; y_centr
pop CX
pop BX
pop AX
in; ASCII code
pop EX
push 30; row length
pop DX
push 0
pop FX; curr index
FILL:
        push FX
        push DX
        mod
        push BX
        sub
        sqr
        push FX
        push DX
        idiv
        push CX
        sub
        sqr
        add
        push AX
        sqr
        jge PRINT
        push 1
        push FX
        add
        pop FX
        push FX
        push DX
        sqr
        jne FILL
        draw
        hlt
PRINT:
        push EX
        pop [FX]
        push 1
        push FX
        add
        pop FX
        jmp FILL