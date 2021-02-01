; @author = ngx123
; @brief = code for the initial setup of the x86-32 platform


%include "boot/multiboot2/multiboot2_bootstrap.s"


; @brief = loads GDT
; @param = address of GDT struct
global gdtLoadAsm       ; If errors - gdtLoadAsm:function

extern display_gdt
extern kernel_main


kernel_setup:
        call kernel_main
kernel_hang:
        hlt
        jmp hang               ; Loop forever


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