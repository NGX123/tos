; File: gdt.s
; Description: includes GDT data structure and pointer

; Function to load the global descriptor table
load_gdt_asm:
        cli
        lgdt [gdt_descriptor]
        jmp 0x08:flush ; 0x08 points to the code segment

                ; Label for the far jump
                flush:
                        ; Reload segment registers
                        mov   ax, 0x10 ; 0x10 points at the new data selector
                        mov   ds, ax
                        mov   es, ax
                        mov   fs, ax
                        mov   gs, ax
                        mov   ss, ax
                        ret



; A label to calculate size of gdt from start to end
gdt_start:

; A null descriptor 
gdt_null:
        dd 0x0  ; 4 bytes of zeros
        dd 0x0 

; Code segement descriptor
gdt_code:
        dw 0xffff       ; Limit(bits 0-15)
        dw 0x0          ; Base(bits 0-15)
        
        db 0x0          ; Base (bits 16-23)
        db 10011010b    ; 1st flags, type flags
        db 11001111b    ; 2nd flags, Limit (bits 16-19)
        db 0x0          ; Base (bits 24-31)

; Data segment descriptor
gdt_data:
        dw 0xffff       ; Limit(bits 0-15)
        dw 0x0          ; Base(bits 0-15)

        db 0x0          ; Base (bits 16-23)
        db 10010010b    ; 1st flags, type flags
        db 11001111b    ; 2nd flags, Limit (bits 16-19)
        db 0x0          ; Base (bits 24-31)

; End of the gdt(for size calculation)
gdt_end:

gdt_descriptor:
        dw gdt_end - gdt_start - 1      ; Size of GDT entries minus 1, needed to be given to the instruction to not overflow the size
        dd gdt_start                    ; Start address(pointer) to GDT, gives the start of the global descriptor table to count until end