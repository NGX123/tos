; Defines
MBALIGN equ 1 << 0              ; align loaded modules on page boundaries
MEMINFO equ 1 << 1              ; provide memory map
FLAGS   equ MBALIGN | MEMINFO   ; muliboot 'flag'
MAGIC   equ 0x1BADB002          ; magic number to let bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot




;;; Multiboot(grub) section ;;;
section .multiboot 

; Writes special data aligned to 3 bytes according to the multiboot standard for programm to be loaded by GRUB
align 4
    dd MAGIC 
    dd FLAGS
    dd CHECKSUM




;;; Stack Section ;;;
section .bss 

; Stack requires to be 16 bytes aligned, then 16 kib are allocated for the stack
align 16
stack_bottom:
; Allocate 16 kilobytes
resb 16384 
stack_top:




;;; Code section ;;;
section .text 

; Linker specifies _start as the entry to the kernel, bootloader jumps here once his work is done(loads us into 32-bit protected mode)
global _start:function ;(_start.end - _start) ; add if something does not work
_start:
        ; Setup stack by loading memory address of the top of the stack into stack register 
        mov esp, stack_top

        cli
        lgdt [gdt_descriptor]
        jmp 0x08:flush

flush:
        ; Call external C function
        extern kernel_main
        call kernel_main

; Put the system into infinite loop
.hang:  hlt
        jmp .hang
.end:




;;; GDT ;;;
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
        dw gdt_end - gdt_start - 1      ; Calculation of size of GDT - 1(A rule)
        dd gdt_start                    ; Start address(pointer) to GDT