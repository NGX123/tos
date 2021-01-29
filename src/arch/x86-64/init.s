; @author = ngx123
; @brief = code for initial setup of the 64 bit UEFI Kernel


global _start


MAGIC       equ 0xE85250D6
ARCH_x86_32 equ 0


section .multiboot
multiboot_header_start:
    align 8
        dd  MAGIC
        dd  ARCH_x86_32
        dd  multiboot_header_end - multiboot_header_start
        dd  -(MAGIC + ARCH_x86_32 + (multiboot_header_end - multiboot_header_start) + 8)
multiboot_header_end:


section .text
_start:
    hlt
    jmp _start