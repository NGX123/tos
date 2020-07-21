// File: idt.c
// Description: c setup of interrupt descriptor table, the names of irqs must also be added here for usage. outb(0xA0, 0x20); needs to be added before out(0x20, 0x20); on interrupts 8+

#define IDT_COUNT 256
#include "../kernel/x86.h"
extern void load_idt();
extern int irq0();

// Defines a data structure for IDT
struct idt_entry{
    unsigned short offset_lowerbits;
    unsigned short selector;

    unsigned char zero;
    unsigned char flags;
    unsigned short offset_higherbits;

} __attribute__((packed));

// Defines a pointer to the IDT data structure
struct idt_pointer{
    unsigned short size;
    unsigned long address;
} __attribute__((packed));




// Initializations
struct idt_entry IDT[IDT_COUNT];
struct idt_pointer ip;

// Initializes the interrupt descriptor table entries 
void idt_init(){
    // Remap the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11); 
    outb(0x21, 0x20); 
    outb(0xA1, 40); 
    outb(0x21, 0x04); 
    outb(0xA1, 0x02); 
    outb(0x21, 0x01);     
    outb(0xA1, 0x01); 
    outb(0x21, 0x0); 
    outb(0xA1, 0x0);




    // Initialize the IDT entry with values
	IDT[32].offset_lowerbits = (unsigned long)irq0 & 0xffff;
	IDT[32].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[32].zero = 0;
	IDT[32].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[32].offset_higherbits = ((unsigned long)irq0 & 0xffff0000) >> 16;

    ip.size = (sizeof(struct idt_entry) * IDT_COUNT) - 1;
    ip.address = (unsigned long)IDT;

    load_idt((unsigned long)&ip);
}


// C functions that handle interrupts
void irq0_handler(void) {
    outb(0x20, 0x20); //EOI
}
 
