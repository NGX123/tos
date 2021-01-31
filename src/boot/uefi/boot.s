; @author = ngx123
; @brief = x86-64-efi platform init file that takes over after bootloader


; Kernel start called by grub(_start is specified in linker.ld)
global _start


; Kernel start called by grub(_start is specified in linker.ld)
MAGIC       equ 0xE85250D6  ; Magic number used by GRUB to detect the executable is multiboot2 compatible
ARCH_x86_32 equ 0           ; Architecture that is used by executbale, 0 - i386


section .multiboot          ; Has data located that will be used by grub to find the executable
multiboot_header_start:
    align 8
        dd  MAGIC
        dd  ARCH_x86_32
        dd  multiboot_header_end - multiboot_header_start                               ; Size of the header
        dd  -(MAGIC + ARCH_x86_32 + (multiboot_header_end - multiboot_header_start))    ; Checksum
multiboot_header_end:


section .text
_start:

hang:                   ; Loop forever
    hlt
    jmp _start