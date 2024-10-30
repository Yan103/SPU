push 5
push AX
push [7]
push [CX] ;!
push [DX + 10] ;!
push AX + 65
add
pop AX
pop [20]
pop [BX]
pop [CX + 10]
hlt