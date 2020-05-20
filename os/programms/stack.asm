mov ah, 0x0e    ; tty BIOS routine

mov bp, 0x8000  ; Set the base of the stack above where BIOS loads boot sector
mov sp, bp      

push 'A'        ; Push some characters on the stack for later retrival(16-bit values)
push 'B'        
push 'C'

pop bx          ; we can pop only 16 bits, so first copy full to bx and then copy the 8 bits
mov al, bl      ; that correspond to ASCII into al 
int 0x10        ; print al

pop bx          ; Pop the next value
mov al, bl
int 0x10

mov al, [0x7ffe] ; 
int 0x10

jmp $


times 510-($-$$) db 0
dw 0xaa55
