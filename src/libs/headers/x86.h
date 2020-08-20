// File: x86.h
// Description: includes defines to use with the x86 CPUs



#ifndef X86_H
#define X86_H

#include <stdint.h>

/// GDT ///
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
    uint64_t base;
} __attribute__((packed));


/// IDT ///
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
    uint64_t address;
} __attribute__((packed));



/// PORT I/O ///
// One byte port reading
static inline uint8_t inb(uint16_t port){
    uint8_t data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

// One byte port outputting
static inline void outb(uint16_t port, uint8_t data){
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

// Word port inputting
static inline uint16_t inw(uint16_t port){
    uint16_t data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

// Word port outputting
static inline void outw(uint16_t port, uint16_t data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}
#endif

