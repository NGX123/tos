; Copy a value into register/memory - mov <dst>, <src>
mov ax, 12

; Jump to a memory address - jmp <address>, in this case current
jmp $

; Interrupt sequence - int <sequence>
int 0x10

; Define word - dw <2 bytes>
dw 'X'          

; Contents of an address [address]
mov ax, [label]

; Add - add <to where>, <what>
add ax, bx

; Declare bytes - db <bytes>, declares the bytes in the memory
label: 
    db 'Booting os' ,0 ; here we decalre a string, last byte is zero so we know where the finishe is
    


; REGISTERS ;
; CPU has 4 registers that each can hold a word - ax, cx, dx, bx
; Modifying register value
mov ax, 1234    ; store 1234 in ax regitser
mov cx, 0x234   ; store 234 hex in cx regitster
mov dx, 't'     ; store ASCII code for 't' in dx
mov bx, ax      ; copy value of ax into bx 


; Setting high and low bits ofn register
mov ax, 0       ; copy zero into register - 0x0000
mov ah, 0x56    ; move 0x56 into first 8 bits of register - 0x5600
mov al, 0x23    ; move 0x23 into last 8 bits of register - 0x5623


; INTERRUPTS ;
; Display letter
mov ah, 0x0e            ; int 10/ah = 0eh - scrolling teletype BIOS routine, we set 8 bits
                        ;   of ax register to indicate it 
mov al, 'H'             ; store the ASCII code of the character to print in the other 8 bits
int 0x10                ; interrupt that prints out character stored in ax register
