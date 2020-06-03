; Shows cases of addressing by printing x in different ways
; Offsetting in 16bit mode

mov ah, 0x0e ; int 10/ ah = 0eh -> scrolling teletype BIOS routine

; Set the al with X and print it
mov al, [the_secret]
int 0x10                ; Does this print an X?

; Set ds offset register
mov bx, 0x7c0           ; Can’t set ds directly, so set bx
mov ds, bx              ; set ds through bx
mov al, [the_secret]    ; here it uses ds(it is defualt)
int 0x10                


; Use the extra offset register
mov bx, 0x7c0           ; set the bx first
mov es, bx              ; set es through bx
mov al, [es:the_secret] ; here it uses special es register
int 0x10                


jmp $   

the_secret :
    db "X"

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55