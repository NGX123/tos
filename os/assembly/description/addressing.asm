; SHOWS HOW ADDRESSING WORKS

mov ah, 0x0e    ; int 10/ah = 0eh - scrolling teletype bios routine

; First attempt - does not print x becuase it tries to print the offset and contents of the offset
mov al, the_secret
int 0x10            ; Does this print an X ?

; Second attempt - tries to print contents of the memory, but fails because it treats as we 
; are trying to access from the start of the memory and not the address we are at
mov al, [the_secret]
int 0x10            ; Does this print an X ?

; Third attempt - here we print the add the address we thinck we are at 0x7c00 to the offset 
;   address and print the value of the address we got from adding them
mov bx, the_secret
add bx, 0x7c00 
mov al, [bx]
int 0x10            ; Does this print an X ?

; Fourth attempt - here we just count how far offset is from the address, and print it's value
mov al, [0x7c1e] 
int 0x10            ; Does this print an X ?

jmp $

the_secret:         ; labels can be declared anywhere and used from anywhere
    db "X"

times 510-($-$$) db 0
dw 0xaa55