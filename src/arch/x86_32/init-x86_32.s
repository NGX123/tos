; @author = ngx123
; @brief = code for the initial setup of the x86_32 platform


%include "boot/multiboot2/multiboot2_bootstrap.s"


; @brief = loads GDT
; @param = address of GDT struct
global gdtLoadAsm       ; If errors - gdtLoadAsm:function

extern kernel_main
extern makeMemFlat


kernel_setup:
        call makeMemFlat
        call kernel_main

        hlt
        jmp $


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

                ret


; Assembly interrupt code declarations
%include "arch/x86_32/platform_interrupts.s"