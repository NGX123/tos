// File: idt.c
// Description: c setup of interrupt descriptor table 

#define IDT_COUNT 1

extern void load_idt(unsigned long);
extern unsigned long isr0;

struct idt_entry{
    unsigned short offset1;
    unsigned short selector;

    unsigned char zero;
    unsigned char flags;
    unsigned short offset2;

} __attribute__((packed));

struct idt_pointer{
    unsigned short size;
    unsigned long address;
} __attribute__((packed));

struct idt_entry IDT[IDT_COUNT];
struct idt_pointer ip;



void idt_init(){

    IDT[0].offset1 = isr0 & 0xffff;
    IDT[0].selector = 0x08;
    IDT[0].zero = 0x0;
    IDT[0].flags = 0xAE;
    IDT[0].offset2 = (isr0 & 0xffff0000) >> 16;
    

    ip.size = (sizeof(struct idt_entry) * IDT_COUNT) - 1;
    ip.address = (unsigned long)IDT;


    load_idt((unsigned long)&ip);
}