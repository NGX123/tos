; To store data we use memory and register


; Using registers 
; CPU has 4 registers that each can hold a word - ax, cx, dx, bx
; Modifying register value
mov ax, 1234    ; store 1234 in ax regitser
mov cx, 0x234   ; store 234 hex in cx regitster
mov dx, 't'     ; store ASCII code for 't' in dx
mov bx, ax      ; copy value of ax into bx 

; Setting high and low bits of register
mov ax, 0       ; copy zero into register - 0x0000
mov ah, 0x56    ; move 0x56 into first 8 bits of register - 0x5600
mov al, 0x23    ; move 0x23 into last 8 bits of register - 0x5623



; Using memory 
; STACK - you have a stack which is a place in memory you choose, and you can put different values on it and then take of(but only the item on top), we use it to not bother with memory adresses and just use it as a pile of data for temp storage
; Stack grows down in the memory, we can put only one word on stack

; Stack uses two registers to set it bp - to put the memory address in temporarily, sp - sets the base and can be only set by bp
mov bp, 0x8000  ; we move the adress where we want to start into bp register to then move it into sp to set stack start
mov sp, bp 

; push <what>, puts ASCII code of A on stack
push 'A' 
; pop <where>, moves what is on top of the stack(A) into bx register
pop bx

; pusha, backup all register values by putting them on the stack
pusha
; popa, restores original values of registers if nothing else was put or taken from stack
popa



; Memory adresses
; Memory adresses are hex numbers, to acces them [address] is used
mov [0x5555], a

; You can store the hex anywhere and then just put [] around storge place
mov ax, 0x5fe3
mov [ax], 605