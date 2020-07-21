; File: boot.s
; Description: the main assembly file that initializes and starts everything



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
global _start:function ; (_start.end - _start) ; add if something does not work
_start:
        ; Setup stack by loading memory address of the top of the stack into stack register 
        mov esp, stack_top
        
        ; Call external C function
        extern kernel_main
        call kernel_main
        
        int 0x00

; Infinite loop
.hang:  hlt
        jmp .hang
.end:




; ------------------ ;
;;; DECLARATIONS ;;;
global load_gdt:function
global load_idt:function
global isr0
extern display_idt
extern display_gdt
extern software_int_test

;; C FUNCTION DECLARATIONS ;;
; Loads Global descriptor table and is a function
load_gdt:
        cli
        mov edx, [esp + 4]
        lgdt[edx]
        jmp 0x08:flush2

        flush2:
                ; Reload segment registers
                mov   ax, 0x10 ; 0x10 points at the new data selector
                mov   ds, ax
                mov   es, ax
                mov   fs, ax
                mov   gs, ax
                mov   ss, ax

                call display_gdt
                ret

; Loads Interrupt Descriptor Table and is a function
load_idt:
	mov edx, [esp + 4]
	lidt[edx]
        ;sti
        call display_idt
	ret

isr0:                 
	call software_int_test
	iret


;; ASSEMBLY EXTERNAL FILES ;;
; GDT code
;%include "init/gdt.s"


; IDT code
;%include "init/idt.s"




