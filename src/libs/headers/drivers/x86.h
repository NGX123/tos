// File: x86.h
// Description: includes defines to use with the x86 CPUs



#ifndef X86_H
#define X86_H

#include <stdint.h>

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
