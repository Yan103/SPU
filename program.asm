push 100
pushr AX
push 60
push 10
jmp SPGOOAL:
loop:
    push 10
    add
    jne loop
popr AX
mlt
SPGOOAL:
out
hlt