;Boot sector that prints a message to the screen using ISR

[org 0x7c00]

mov bx, HELLO_MSG
call print_string

mov bx, GOODBYE_MSG
call print_string

jmp $



print_string:
    pusha
    mov ah, 0x0e 
    ; Move the value of bx - first letter of string into the al register
    

    print:
        ; Move the value of the address into al, first the address is the address of the first
        ;   letter(becuase it can not point to the whole string and only to the start) then it 
        ;   uses one of the next commands to move to the next letter
        mov al, [bx]

        ; Print the letter with an interupt
        int 0x10

        ; Move bx to the address of next letter(next byte), by adding 1 to it
        add bx, 1

        ; Compare value of al to 0, to know when we encounterd the end
        cmp al, 0

        ; If the letter is not 0 and it is not the end, it would loop again and print next letter
        jne print

        popa    ; Restore register values
        ret     ; Return to the caller of the functions

; Hello string
HELLO_MSG:
    db 'Hello, World', 0

; Bye string
GOODBYE_MSG:
    db 'Goodbye', 0




times 510-($-$$) db 0
dw 0xaa55 







