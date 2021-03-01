/*
    @author = ngx123
    @brief = idt handler code header
*/


#ifndef IDT_DEFINES_H
#define IDT_DEFINES_H


#include <stdint.h>
#include <arch/io_x86.h>
#include <kernel/interrupts.h>
#include "stdio.h"


#define IDT_COUNT 256


struct idt_entry
{
    uint16_t offset_lowerbits;
    uint16_t selector;

    uint8_t zero;
    uint8_t flags;
    uint16_t offset_higherbits;

} __attribute__((packed));

struct idt_pointer
{
    uint16_t size;
    void* address;
} __attribute__((packed));


/*
    @brief = PIC Remapping
*/
static void picRemap();

/*
    @brief = ASM Routine to load IDT into memory
*/
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
void irq0_handler(void);
void irq1_handler(void);
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);
#endif