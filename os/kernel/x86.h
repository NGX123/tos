// File: x86.h
// Description: includes functions and defines to use with the x86 CPUs



static inline unsigned char inb(unsigned short port){
    unsigned char data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data){
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

