; @author = ngx123
; @brief = platform init file that takes over after bootloader


; Kernel start called by grub(_start is specified in linker.ld)
global _start   ; If errors - _start:function

extern setBootInfo
extern kernel_main


MBALIGN         equ 1 << 0              ; Loaded modules page boundaries aligning flag
MEMINFO         equ 1 << 1              ; Memory map flag
FLAGS           equ MBALIGN | MEMINFO   ; GRUB boot flags combination
MAGIC           equ 0x1BADB002          ; Value for grub detection
CHECKSUM        equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot


section .multiboot              ; GRUB section, includes writing special data at start of binary for grub to see the OS
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .bss
align 16                        ; Aligning is required for stack
        stack_bottom:
                resb 16384      ; Allocate 16 kilobytes for stack
        stack_top:


section .text
_start:
        mov esp, stack_top      ; Load stack address into stack register(initialize)

        push ebx
        push eax
        push 2
        call setBootInfo        ; kernel_init(var_num, mboot_magic_num, mboot_struct)

        call kernel_main


.hang:
        hlt
        jmp .hang               ; Loop forever
