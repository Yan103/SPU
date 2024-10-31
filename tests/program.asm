push 1
push 1
pop BX
pop CX
in; input a number
pop AX
call FACTORIAL; call factorial function
push CX
out
hlt      ; end of the programm
NEXT:
        push CX
        mul
        pop CX
        pop BX
        push AX
        push 1
        sub
        pop AX
        call FACTORIAL
FACTORIAL:
        push BX
        push AX
        jne NEXT
        ret