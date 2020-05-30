; In assembly there is no logical statements or loop instruction, everything is done by hand


; cmp <this>, <to this> - compares 1 argument to second and sets a certain cpu flag with the result
cmp ax, 4

; Then we use certain jump instruction to jump on a certain result of the comparasion

; je <label>, jump if they are equal
je target

; jump if not equal ( i.e. x != y)
jne target 

; jump if less than ( i.e. x < y)
jl target 

; jump if less than or equal ( i.e. x <= y)
jle target 

; jump if greater than ( i.e. x > y)
jg target 

; jump if greater than or equal ( i.e. x >= y)
jge target 


; loops could be made by jumping to the same address over and over and if statements with cmp