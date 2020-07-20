// File: idt.c
// Desription: Includes defenitions and functions for the global descriptor table



#include "../kernel/x86.h"
#define IDT_SIZE 256
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
*/	

extern unsigned long keyboard_handler;
extern void load_idt(unsigned long idt_ptr);

// Defining a IDT entry structure
struct IDT_entry{
	unsigned short offset_lowerbits;
	unsigned short selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short offset_higherbits;
};

// A list of structures
struct IDT_entry IDT[IDT_SIZE];



void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	// populate IDT entry of keyboard's interrupt 
	keyboard_address = (unsigned long)keyboard_handler; 
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET 
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; // INTERRUPT_GATE 
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	

	

	// ICW1 - begin initialization 
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	// ICW2 - remap offset address of IDT. In x86 protected mode, we have to remap the PICs beyond 0x20 because. Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	// ICW3 - setup cascading 
	outb(0x21 , 0x00);  
	outb(0xA1 , 0x00);  

	// ICW4 - environment info 
	outb(0x21 , 0x01);
	outb(0xA1 , 0x01);
	// Initialization finished 

	// mask interrupts 
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);

	// fill the IDT descriptor 
	idt_address = (unsigned long)IDT;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16;

	load_idt((unsigned long)idt_ptr);
}

void keyboard_handler_main(void) {
	unsigned char status;
	char keycode;
	volatile char* vidptr = (volatile char*)0xb8000;
	*vidptr++;

	/* write EOI */
	outb(0x20, 0x20);

	status = inb(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inb(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return;
		*vidptr++ = 'a'; //keyboard_map[keycode];
		*vidptr++ = 0x07;	
	}
}