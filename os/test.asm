mov bx, 30
mov ah, 0x0e

cmp bx, 4
jle then1
jl then2
mov al, 'C'
jmp the_end



then1:
    mov al, 'A'

then2:
    mov al, 'B'

the_end:

int 0x10

jmp $


times 510-($-$$) db 0
dw 0xaa55