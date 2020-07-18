// File: idt.c
// Description: c setup of interrupt descriptor table 

#define IDT_COUNT 1

extern void load_idt(unsigned long);
unsigned long isr;
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

void idt_init(){
    for (int i = 0; i < IDT_COUNT; i++){
        //IDT[i].offset1
        IDT[i].selector = 0x08;
        IDT.zero = 0x0;
        IDT.flags = 
    }
}