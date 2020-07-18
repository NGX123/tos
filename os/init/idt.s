; Loads interrupt descriptor table
load_idt_asm:
        sti
        lidt [idt_descriptor]
        sti
        ret


; Start address 
idt_start:

; First IRQ
irq0:
        dw      isr0            ; offset, split into two parts address of ISR entry point (0-15)
        dw      0x0008          ; segment selector points to descriptor in GDT or LDT
        db      0x0             ; set to 0 always 
        db      10101110b     ; type and attr flags 
        dw      0x0000          ; offset bits (16-31)


idt_finish:

; Descriptor(pointer) for loading into IDT register
idt_descriptor:
        dw idt_start - idt_finish - 1
        dd idt_start




; Service routine 0 - will be called if first interrupt is called 
