; In Asembly to do functions, while loops and much more you
;   need to know how to go to a cetain memory address in your programm 



; Label is a name for an address in a programm
label: 
    ; jmp <address>, go to that memory
    jmp label 


; Optional jump instructions
; cmp <element>, <element>, compares them and sets the needed flag
cmp ax, bx

je label  ; jump if they are equal(in cmp)
jne label ; jump if not equal
jl label  ; jump if less than (x < y)
jle label ; jump if less them or equal
jg label  ; jump if greater than
jge label ; jump if greater the or equal