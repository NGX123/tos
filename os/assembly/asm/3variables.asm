; To store wore, sentances or even words we make a label to memory and write direct bytes of letters with 0 at the end
; 0 is written in the end of the string so we could understand where in memory it ends


; label; db <data>, writes binary of <data> directly 
var_label: db 'Hello, World' ,0

; label; dw <data>, writes two bytes of <data> directly
var_label: dw 'HW' ,0 


; The label stores only the first letter of the variable, then we need to add 1 to memory to get further, and check if number is zero finish
mov bx, var_label
mov ah, [bx]

; Adds one to memory address in bx to switch to next letter
add [bx], 1