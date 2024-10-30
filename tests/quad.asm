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
        pushr DX
        call answer
        ret
answer:
        jz 1root
        jgz 2root
        jlz 0root
        0root:
                push -666
                out
                ret
        1root:
                push -1
                popr BX
                mlt
                push 2
                popr AX
                mlt
                div
                out
                ret
        2root:
                push -1
                popr BX
                mlt
                pushr BX


                ret