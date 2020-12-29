// File: idt.h
// Description: idt handler code header

#ifndef IDT_DEFINES_H
#define IDT_DEFINES_H

#include <stdint.h>
#include "drivers/x86.h"

#define IDT_COUNT 256


// IDT data structure
struct idt_entry{
    uint16_t offset_lowerbits;
    uint16_t selector;

    uint8_t zero;
    uint8_t flags;
    uint16_t offset_higherbits;

} __attribute__((packed));

// IDT data structure pointer(in form of struct)
struct idt_pointer{
    uint16_t size;
    void* address;
} __attribute__((packed));


// PIC Remapping
extern void picRemap();

// IDT intialization
extern void idtInit();

// IDT loading ASM routine
extern void idtLoad();

// ASM interrupt handler addresses
extern int irq0_handler_asm();
extern int irq1_handler_asm();
extern int irq2_handler_asm();
extern int irq3_handler_asm();
extern int irq4_handler_asm();
extern int irq5_handler_asm();
extern int irq6_handler_asm();
extern int irq7_handler_asm();
extern int irq8_handler_asm();
extern int irq9_handler_asm();
extern int irq10_handler_asm();
extern int irq11_handler_asm();
extern int irq12_handler_asm();
extern int irq13_handler_asm();
extern int irq14_handler_asm();
extern int irq15_handler_asm();

// C Interrupt handlers
extern void irq0_handler();
extern void irq1_handler();
extern void irq2_handler();
extern void irq3_handler();
extern void irq4_handler();
extern void irq5_handler();
extern void irq6_handler();
extern void irq7_handler();
extern void irq8_handler();
extern void irq9_handler();
extern void irq10_handler();
extern void irq11_handler();
extern void irq12_handler();
extern void irq13_handler();
extern void irq14_handler();
extern void irq15_handler();

// Keyboard interrupt handler
extern void keyboard_handler();
#endif