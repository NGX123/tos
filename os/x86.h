// File: x86.h
// Description: includes functions and defines to use with the x86 CPUs
// Problems:



static inline unsigned char inb(unsigned short port){
    unsigned char data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data){
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void lgdt(struct gdt *p, int size)
{
  volatile uint16_t pd[3];

  pd[0] = size-1;
  pd[1] = (uint32_t)p;
  pd[2] = (uint32_t)p >> 16;

  asm volatile("lgdt (%0)" : : "r" (pd));
}

