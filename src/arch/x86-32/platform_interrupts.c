/*
    @author = ngx123
    @brief = c setup of interrupt descriptor table, the names of irqs must also be added here for usage. outb(0xA0, 0x20); needs to be added before out(0x20, 0x20); on interrupts 8+
*/


#include "headers/platform_interrupts.h"


static struct idt_entry IDT[IDT_COUNT];
static struct idt_pointer ip;
static interrupt_interpreter_func_t interruptInterpreter;


int requestReservedInterrupts(int* numbers, int list_size){
	int i;
	int reserved_interrupts_list_size = 1;
	int reserved_interupts_list[reserved_interrupts_list_size];
	reserved_interupts_list[0] = 0;

	if (reserved_interrupts_list_size > list_size)
		return -1;
	else
		for (i = 0; i < reserved_interrupts_list_size; i++)
			numbers[i] = reserved_interupts_list[i];

	return i;
}

int setInterruptInterpreterFunction(interrupt_interpreter_func_t function){
	interruptInterpreter = function;

	return 0;
}

static void picRemap(){
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
}

void platformInterruptsInit(){
    picRemap();

    // Write the entries into IDT
	IDT[32].offset_lowerbits = (uint32_t)irq0_handler_asm & 0xffff;
	IDT[32].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[32].zero = 0;
	IDT[32].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[32].offset_higherbits = ((uint32_t)irq0_handler_asm & 0xffff0000) >> 16;

    IDT[33].offset_lowerbits = (uint32_t)irq1_handler_asm & 0xffff;
	IDT[33].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[33].zero = 0;
	IDT[33].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[33].offset_higherbits = ((uint32_t)irq1_handler_asm & 0xffff0000) >> 16;

    IDT[34].offset_lowerbits = (uint32_t)irq2_handler_asm & 0xffff;
	IDT[34].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[34].zero = 0;
	IDT[34].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[34].offset_higherbits = ((uint32_t)irq2_handler_asm & 0xffff0000) >> 16;

    IDT[35].offset_lowerbits = (uint32_t)irq3_handler_asm & 0xffff;
	IDT[35].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[35].zero = 0;
	IDT[35].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[35].offset_higherbits = ((uint32_t)irq3_handler_asm & 0xffff0000) >> 16;

    IDT[36].offset_lowerbits = (uint32_t)irq4_handler_asm & 0xffff;
	IDT[36].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[36].zero = 0;
	IDT[36].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[36].offset_higherbits = ((uint32_t)irq4_handler_asm & 0xffff0000) >> 16;

    IDT[37].offset_lowerbits = (uint32_t)irq5_handler_asm & 0xffff;
	IDT[37].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[37].zero = 0;
	IDT[37].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[37].offset_higherbits = ((uint32_t)irq5_handler_asm & 0xffff0000) >> 16;

    IDT[38].offset_lowerbits = (uint32_t)irq6_handler_asm & 0xffff;
	IDT[38].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[38].zero = 0;
	IDT[38].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[38].offset_higherbits = ((uint32_t)irq6_handler_asm & 0xffff0000) >> 16;

    IDT[39].offset_lowerbits = (uint32_t)irq7_handler_asm & 0xffff;
	IDT[39].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[39].zero = 0;
	IDT[39].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[39].offset_higherbits = ((uint32_t)irq7_handler_asm & 0xffff0000) >> 16;

    IDT[40].offset_lowerbits = (uint32_t)irq8_handler_asm & 0xffff;
	IDT[40].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[40].zero = 0;
	IDT[40].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[40].offset_higherbits = ((uint32_t)irq8_handler_asm & 0xffff0000) >> 16;

    IDT[41].offset_lowerbits = (uint32_t)irq9_handler_asm & 0xffff;
	IDT[41].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[41].zero = 0;
	IDT[41].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[41].offset_higherbits = ((uint32_t)irq9_handler_asm & 0xffff0000) >> 16;

    IDT[42].offset_lowerbits = (uint32_t)irq10_handler_asm & 0xffff;
	IDT[42].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[42].zero = 0;
	IDT[42].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[42].offset_higherbits = ((uint32_t)irq10_handler_asm & 0xffff0000) >> 16;

    IDT[43].offset_lowerbits = (uint32_t)irq11_handler_asm & 0xffff;
	IDT[43].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[43].zero = 0;
	IDT[43].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[43].offset_higherbits = ((uint32_t)irq11_handler_asm & 0xffff0000) >> 16;

    IDT[44].offset_lowerbits = (uint32_t)irq12_handler_asm & 0xffff;
	IDT[44].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[44].zero = 0;
	IDT[44].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[44].offset_higherbits = ((uint32_t)irq12_handler_asm & 0xffff0000) >> 16;

    IDT[45].offset_lowerbits = (uint32_t)irq13_handler_asm & 0xffff;
	IDT[45].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[45].zero = 0;
	IDT[45].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[45].offset_higherbits = ((uint32_t)irq13_handler_asm & 0xffff0000) >> 16;

    IDT[46].offset_lowerbits = (uint32_t)irq14_handler_asm & 0xffff;
	IDT[46].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[46].zero = 0;
	IDT[46].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[46].offset_higherbits = ((uint32_t)irq14_handler_asm & 0xffff0000) >> 16;

    IDT[47].offset_lowerbits = (uint32_t)irq15_handler_asm & 0xffff;
	IDT[47].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[47].zero = 0;
	IDT[47].flags = 0x8e; /* INTERRUPT_GATE */
	IDT[47].offset_higherbits = ((uint32_t)irq15_handler_asm & 0xffff0000) >> 16;

    ip.size = (sizeof(struct idt_entry) * IDT_COUNT) - 1;
    ip.address = (void*)IDT;

    idtLoadAsm((void*)&ip);
}


// C Interrupt Handlers
void irq0_handler(){
    outb(0x20, 0x20); // EOI(End of Interrupt)
}

void irq1_handler(){
	interruptInterpreter(1);
    outb(0x20, 0x20);
}

void irq2_handler(){
	interruptInterpreter(2);
    outb(0x20, 0x20);
}

void irq3_handler(){
	interruptInterpreter(3);
    outb(0x20, 0x20);
}

void irq4_handler(){
	interruptInterpreter(4);
    outb(0x20, 0x20);
}

void irq5_handler(){
	interruptInterpreter(5);
    outb(0x20, 0x20);
}

void irq6_handler(){
	interruptInterpreter(6);
    outb(0x20, 0x20);
}

void irq7_handler(){
	interruptInterpreter(7);
    outb(0x20, 0x20);
}

void irq8_handler(){
	interruptInterpreter(8);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq9_handler(){
	interruptInterpreter(9);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq10_handler(){
	interruptInterpreter(10);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq11_handler(){
	interruptInterpreter(11);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq12_handler(){
	interruptInterpreter(12);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq13_handler(){
	interruptInterpreter(13);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq14_handler(){
	interruptInterpreter(14);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

void irq15_handler(){
	interruptInterpreter(15);
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
}