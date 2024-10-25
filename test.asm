call quad
hlt
quad:
        in
        cpy
        pushr AX
        in
        cpy
        cpy
        mlt
        pushr EX
        pushr BX
        in
        push 4
        mlt
        mlt
        pushr CX
        call solution
        ret
solution:
        popr EX
        popr CX
        sub
        out
        ret