// File: idt.h
// Description: idt handler code header

/// Includes
#include <stdint.h>
#include "drivers/x86.h"

#define IDT_COUNT 256



/// Structure declarations
// Defines a data structure for IDT
struct idt_entry{
    uint16_t offset_lowerbits;
    uint16_t selector;

    uint8_t zero;
    uint8_t flags;
    uint16_t offset_higherbits;

} __attribute__((packed));

// Defines a pointer to the IDT data structure
struct idt_pointer{
    uint16_t size;
    void* address;
} __attribute__((packed));



/// External Declarations
extern void keyboard_handler();
extern void load_idt();
extern int irq0();
extern int irq1();



/// Declarations
// Initializes the interrupt descriptor table entries
void idt_init();

// C functions that handle interrupts
void irq0_handler(void);
void irq1_handler(void);