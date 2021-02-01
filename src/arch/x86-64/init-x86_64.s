; @author = ngx123
; @brief = code for the initial setup of the x86-64 platform


%include "boot/multiboot2/multiboot2_bootstrap.s"


kernel_setup:

kernel_hang:
        hlt
        jmp hang               ; Loop forever