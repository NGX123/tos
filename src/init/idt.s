; File: idt.s
; Description: includes the assembly part of the irq handlers, to add new just changes the <NUM> in irq<NUM> for other number in all occurences



; Expsoing irq addresses to be called
global irq0

; External c functions for handing interrupts
extern irq0_handler

; Assembly functions hndlers
irq0:  
    pusha               
    call irq0_handler
    popa
    iret

