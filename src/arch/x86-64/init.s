; @author = ngx123
; @brief = code for initial setup of the 64 bit UEFI Kernel


global _start


; Kernel start called by grub(_start is specified in linker.ld)
MAGIC       equ 0xE85250D6  ; Magic number used by GRUB to detect the executable is multiboot2 compatible
ARCH_x86_32 equ 0           ; Architecture that is used by executbale, 0 - i386


section .text
multiboot_header_start: ; Writes 64-bit aligned data directly to the start of executble so grub can find the header
    align 8
        dd  MAGIC
        dd  ARCH_x86_32
        dd  multiboot_header_end - multiboot_header_start
        dd  -(MAGIC + ARCH_x86_32 + (multiboot_header_end - multiboot_header_start))    ; Checksum
multiboot_header_end:


_start:
    hlt
    jmp _start