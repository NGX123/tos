; @author = ngx123
; @brief = includes the assembly part of the irq handlers, to add new just changes the <NUM> in irq<NUM> for other number in all occurences


; @brief = loads IDT
; @param = address of the IDT struct
global idtLoadAsm   ; If errors - idtLoadAsm:function


extern irq0_handler
extern irq1_handler
extern irq2_handler
extern irq3_handler
extern irq4_handler
extern irq5_handler
extern irq6_handler
extern irq7_handler
extern irq8_handler
extern irq9_handler
extern irq10_handler
extern irq11_handler
extern irq12_handler
extern irq13_handler
extern irq14_handler
extern irq15_handler

global irq0_handler_asm
global irq1_handler_asm
global irq2_handler_asm
global irq3_handler_asm
global irq4_handler_asm
global irq5_handler_asm
global irq6_handler_asm
global irq7_handler_asm
global irq8_handler_asm
global irq9_handler_asm
global irq10_handler_asm
global irq11_handler_asm
global irq12_handler_asm
global irq13_handler_asm
global irq14_handler_asm
global irq15_handler_asm


idtLoadAsm:
	mov edx, [esp + 4]
    lidt[edx]
    sti
	ret


irq0_handler_asm:
    pusha
    call irq0_handler
    popa
    iret

irq1_handler_asm:
    pusha
    call irq1_handler
    popa
    iret

irq2_handler_asm:
    pusha
    call irq2_handler
    popa
    iret

irq3_handler_asm:
    pusha
    call irq3_handler
    popa
    iret

irq4_handler_asm:
    pusha
    call irq4_handler
    popa
    iret

irq5_handler_asm:
    pusha
    call irq5_handler
    popa
    iret

irq6_handler_asm:
    pusha
    call irq6_handler
    popa
    iret

irq7_handler_asm:
    pusha
    call irq7_handler
    popa
    iret

irq8_handler_asm:
    pusha
    call irq8_handler
    popa
    iret

irq9_handler_asm:
    pusha
    call irq9_handler
    popa
    iret

irq10_handler_asm:
    pusha
    call irq10_handler
    popa
    iret

irq11_handler_asm:
    pusha
    call irq11_handler
    popa
    iret

irq12_handler_asm:
    pusha
    call irq12_handler
    popa
    iret

irq13_handler_asm:
    pusha
    call irq13_handler
    popa
    iret

irq14_handler_asm:
    pusha
    call irq14_handler
    popa
    iret

irq15_handler_asm:
    pusha
    call irq15_handler
    popa
    iret