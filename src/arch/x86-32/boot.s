%include "boot/bios/boot.s"

global load_gdt:function
global load_idt:function
extern display_idt
extern display_gdt

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
        sti
        call display_idt
	ret


; IDT code
%include "arch/x86-32/idt.s"