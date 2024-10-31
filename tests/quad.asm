push 1
push 1
pop BX
pop CX
in; input a number
pop AX
call FACT; callinf factorial function
push CX;
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
        call FACT ; call next factorial function
FACT:
        push BX
        push AX
        jne NEXT
        ret