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


; MBALIGN         equ 1 << 0              ; Loaded modules page boundaries aligning flag
; MEMINFO         equ 1 << 1              ; Memory map flag
; FLAGS           equ MBALIGN | MEMINFO   ; GRUB boot flags combination
; MAGIC           equ 0x1BADB002          ; Value for grub detection
; CHECKSUM        equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; section .multiboot              ; GRUB section, includes writing special data at start of binary for grub to see the OS
; align 4
;     dd MAGIC
;     dd FLAGS
;     dd CHECKSUM

; section .text
; _start:
;     hlt
;     jmp _start