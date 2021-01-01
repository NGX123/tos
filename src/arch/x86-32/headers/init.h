// File: init.h
// Description: gdt initialization code header

#ifndef GDT_DEFINES_H
#define GDT_DEFINES_H

/// Includes
#include <stdint.h>

// A gdt entry to be loaded using pointer
struct gdt_entry{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// Pointer to the GDT, The max bytes taken up by the GDT, minus 1
struct gdt_ptr{
    uint16_t limit;
    void* base;
} __attribute__((packed));



// Declarations
// Loads the GDT in to the CPU
extern void load_gdt(void*);

// Initializes the flat memory model
void setFlat();
#endif