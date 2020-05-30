; When we have a piece of code that would be repeated multiple times, we do not want to write it over and 
;   ovar again, so we use functions.
; To make a function we need to make a label in memory to which we can go and when they finish go back to out
;   code

; call <function_label>, puts current address on the stack and jumps to the function
call func_label

; Declare function in memory
func_label:
    ; Backup register values
    pusha

    ; Function code - anything
    mov ax, 58
    mov bx, 'A'

    ; Restore register values
    popa

    ; ret - pops the addres from stack and jumps to it
    ret



