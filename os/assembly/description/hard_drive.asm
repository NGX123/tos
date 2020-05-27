; Access to disk happens also by using interupts and setting specific function in the first 
;   8 bits(ah) of the register ax

[org 0x7c00]

mov [BOOR_DRIVE], dl ; bios stores boot drive in DL

mov bp, 0x8000       ; Set the stack out of the way
mov sp, bp

mov bx, 0x9000       ; Load 5 secotrs to 0x0000(ES):0x9000(BX) from the disk
mov dh, 5 
mov dl, [BOOR_DRIVE]
call disk_load

mov dx, [0x9000]      ; print out the first loaded word
call print_hex

mov dx, [0x9000 + 512]  ; print the first word from second sector
call print_hex

jmp $



disk_load:
    push dx         ; put dx on stack to later recall amount of secotrs

    mov ah, 0x02    ; set the read sector function for the interrupt 

    mov al, dh      ; Read DH sectors
    mov dl, 0       ; Read drive 0(first floppy)
    mov ch, 0x00    ; Select cylinder 0
    mov dh, 0x00    ; Select track on 2nd side of floppy
    mov cl, 0x02    ; Start reading from second sector(after the boot sector)

    int 0x13        ; interrupt to start the function on the disk


    ; When we read the disk we need to know if the operation was succesful or if we read random
    ;   bytes of data that have no meaning, we do this by checking the status of the carry flag
    ;   after disk operation

    ; jc address - jumps if the carry_flag was set which means there was an error
    jc disk_error   ; Jump if error

    pop dx
    cmp dh, al      ; if not the same amount of sectors then we read then print error
    jne disk_error
    ret 



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


disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $



; Variable
BOOT_DRIVE: db 0
DISK_ERROR_MSG: db "Disk read error!", 0

times 510-($-$$) db 0
dw 0xaa55

times 256 dw 0 xdada
times 256 dw 0 xface