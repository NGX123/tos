; @author = ngx123
; @brief = code for the initial setup of the platform


%include "boot/bios/boot.s"


; @brief = loads GDT
; @param = address of GDT struct
global gdtLoadAsm:function

extern display_gdt


gdtLoadAsm:
        cli
        mov edx, [esp + 4]
        lgdt[edx]
        jmp 0x08:flush2

        flush2:
                mov   ax, 0x10
                mov   ds, ax
                mov   es, ax
                mov   fs, ax
                mov   gs, ax
                mov   ss, ax

                call display_gdt
                ret


; Assembly interrupt code declarations
%include "arch/x86-32/platform_interrupts.s"