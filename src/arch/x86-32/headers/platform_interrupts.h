// File: idt.h
// Description: idt handler code header

#ifndef IDT_DEFINES_H
#define IDT_DEFINES_H

#include <stdint.h>
#include "drivers/x86.h"
#include "interrupts.h"

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
void picRemap();

// IDT loading ASM routine
extern void idtLoadAsm();

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
void irq0_handler();
void irq1_handler();
void irq2_handler();
void irq3_handler();
void irq4_handler();
void irq5_handler();
void irq6_handler();
void irq7_handler();
void irq8_handler();
void irq9_handler();
void irq10_handler();
void irq11_handler();
void irq12_handler();
void irq13_handler();
void irq14_handler();
void irq15_handler();

// Keyboard interrupt handler
extern void keyboard_handler();
#endif