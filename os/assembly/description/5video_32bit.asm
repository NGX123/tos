; When we switch to the 32 bit mode, there is no longer any routines we can use to manipulate
;   devices, so we write specific values to certain memory locations to use the devices

; In video memory each cell of display is repesented with two bytes, 1st - ASCII code, 2nd -
;   attributes like fore and back ground color
; The video memory starts at address 0xb8000, using which we can callculate right display cell
;   memory location - 0xb8000 + 2 * (row * 80 + col) and put the two bytes there 


; Here is a programm that moves character from strings into memory - prints them, we use
;   the edx register for memory and ebx for the character, everything else is set in the programm


STRING: db 'Hello world', 0
mov ebx, STRING
jmp print_string_pm

; Define constant - NAME equ number, makes a NAME that represents the number
VIDEO_MEMORY equ 0xb8000    ; the start of the video memory to count the right cell
WHITE_ON_BLACK equ 0x0f     ; hex for white on black attribute to use in char bytes

; prints a null-terminated string
print_string_pm:
    pusha                   
    mov edx, VIDEO_MEMORY   ; set edx to where we are in video memory

print_string_pm_loop:
    ; Set up the two character bytes in ax register
    mov al, [ebx]           ; store char in EBX in first byte
    mov ah, WHITE_ON_BLACK  ; store the attributes in second byte

    ; Check if we are in the end of string
    cmp al, 0               ; if al is 0 - end of the string, so we 
    je print_string_pm_done ;   jump to done

    ; Move the character bytes from ax to edx for printing
    mov [edx], ax           ; move the two character bytes in ax register into edx 

    ; Move to next character and cell for the next printing loop
    add ebx, 1              ; move ebx to next character in the string
    add edx, 2              ; move edx to the next cell in video memory

    ; Start the next printing loop
    jmp print_string_pm_loop ; loop around to print next character

; finish the function
print_string_pm_done:
    popa 
    ;ret                     ; return from the functions

jmp $

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55