; File: idt.s
; Description: includes the assembly part of the irq handlers, to add new just changes the <NUM> in irq<NUM> for other number in all occurences



; Expsoing irq addresses to be called
global irq0
global irq1

; External c functions for handing interrupts
extern irq0_handler
extern irq1_handler

; Assembly functions hndlers
irq0:  
    pusha               
    call irq0_handler
    popa
    iret

irq1:
    pusha
    call irq1_handler
    popa
    iret

