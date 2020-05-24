; Includes a file
%include "file.asm"         

; [org address], Tell the assembler where this code will be loaded 
[org 0x7c00]


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
    


; PADDING ; 
times 510-($-$$) db 0   ;  Compiled programm needs to fit in 512 bytes, with 2 reserved for 
                        ;   magic number, so here programm is padded with enough zeros(db 0) 
                        ;   to  get to the 510 byte 
                        
dw 0xaa55               ; fill the last two bytes with magic number



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



; STACK ;
; There are two stack registers sp and bp, you could put 16 bits on the stack per one time
; Push - push <what>, register pushes a word to the stack
push 'A'

; Pop - pop <where>, takes the top of the stack and puts it into the register
pop bx



; IF ; 
; cmp uses cpu flags register, so je is a special jump instruction that works if the certain
; cpu flag is on - it is set by the cmp. There are a lot instructions that execute based on
; flags, like je.
cmp ax, 4                  ; compares the ax value to 4
je then_block              ; jump to then_block if they are equal
mov bx, 45                 ; else exxecute this code
jmp the_end                ; jump over then block 



; FUNCTIONS ;
; The functions work by jumping to address and then coming back 
; call <address>, jump to label, and pushes address onto the stack
call label  

; ret, pops the address from the stack and returns to it               
ret            

; pusha - in the proccess of using a function some registers can be change and this function
;   pushes all of them on the stack to backup
pusha

; popa - restores the original register values
popa