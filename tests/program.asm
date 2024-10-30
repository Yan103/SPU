push 1
push 1  ; 666
pop BX
pop CX
in
pop AX
call FACT
push CX;aboba
out
hlt
NEXT:
        push CX
        mul
        pop CX
        pop BX
        push AX
        push 1
        sub
        pop AX
        call FACT
FACT:
        push BX
        push AX
        jne NEXT
        ret
