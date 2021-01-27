; @author = ngx123
; @brief = code for initial setup of the 64 bit UEFI Kernel


MAGIC       equ 0xE85250D6
ARCH_x86_32 equ 0
HEADER_LEN  equ      24
CHECKSUM    equ      -(MAGIC + ARCH_x86_32 + HEADER_LEN + 8)


section .multiboot
align    8
    dd   MAGIC
    dd   ARCH_x86_32
    dd   HEADER_LEN
    dd   CHECKSUM
    dd   0            ;tag end
    dd   8