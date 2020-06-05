; Defined constants
MBALIGN equ 1 << 0              ; align loaded modules on page boundaries
MEMINFO equ 1 << 1              ; provide memory map
FLAGS   equ MBALIGN | MEMINFO   ; muliboot 'flag'
MAGIC   equ 0x1BADB002          ; magic number to let bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot




; This is a section where special values are written and aligned by 4 bytes so the bootloader
;   could recognize us

; Start a section with headers for multiboot
section .multiboot 

align 4
    dd MAGIC 
    dd FLAGS
    dd CHECKSUM




; The bootlader does not provide a stack, so kernel should do it, so first acording to the 
;   standard the stack needs to be 16 bytes aligned, then 16 kib stack is allocated for
;   later declaration of stack in stack pointer

; Start a section for stack
section .bss 

align 16
stack_bottom:
resb 16384 ; 16 kiB allocated
stack_top:




; Linker specifies _start as the entry to the kernel, so bootloader will jump to this position 
;   once the kernel is loaded

; Main code section
section .text 

global _start:function (_start.end - _start)
_start:
        ; Bootloader loads us into 32bit mode, here we have a full control, but only using
        ;   code that we written - no interups, paging...

        ; Set up the stuck by setting stack poinnter, pointing to the top of out stack
        ;   because it grows downwards. STack is important becuse is can not function without it
        mov esp, stack_top

        extern kernel_main
        call kernel_main

        ; Put the system into infinite loop
        cli
.hang:  hlt
        jmp .hang
.end: