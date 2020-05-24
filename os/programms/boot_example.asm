;Boot sector that prints a message to the screen using ISR

mov ah, 0x0e            ; int 10/ah = 0eh - scrolling teletype BIOS routine, we set 8 bits
                        ;   of ax register to indicate it 

mov al, 'H'             ; store the ASCII code of the character to print in the other 8 bits
int 0x10                ; interrupt that prints out character stored in ax register
mov al, 'e'
int 0x10
mov al, 'l' 
int 0x10
mov al, 'l'
int 0x10
mov al, 'o' 
int 0x10



; jump to the current address(endless)
jmp $                   

; Padding
times 510-($-$$) db 0   ;  Compiled programm needs to fit in 512 bytes, with 2 reserved for 
                        ;   magic number, so here programm is padded with enough zeros(db 0) 
                        ;   to  get to the 510 byte 
                        
dw 0xaa55               ; fill the last two bytes with magic number


