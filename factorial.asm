push 5
call factorial
popr AX
out
hlt
factorial:
    push 1
    mlt
    
    je ret
    call factorial