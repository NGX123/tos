; In assembly function are just a jump to an address(label) and then go back

jmp prints              ; call the function
call prints             ; like jmp, but pushes adress on the stack

prints:
    pusha               ; backup all regitser values
    mov ah, 0x0e        ; move routine into register
    int 0x10            ; print char
    ret                 ; ret pops return address from the stack and jumps to it
    popa                ; restore all register values


times 510-($-$$) db 0   
dw 0xaa55        

