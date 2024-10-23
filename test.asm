push 100
pushr AX; pushing to AX register
push 60
push 10
loop:        ; label start
    push 10
    add
    jne loop
popr AX
mlt
out
hlt