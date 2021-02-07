/*
    @author = ngx123
    @brief = includes defines for port IO on x86
*/


#include <arch/io_x86.h>


void outb(uint16_t port, uint8_t data)
{
    asm volatile ("outb %b0,%w1" : : "a"(data), "d"(port));
}

void outw(uint16_t port, uint16_t data)
{
  asm volatile ("outw %w0,%w1" : : "a"(data), "d"(port));
}

void outl(uint16_t port, uint32_t data)
{
    asm volatile ("outl %0,%w1" : : "a"(data), "d"(port));
}


uint8_t inb(uint16_t port)
{
     uint8_t data;
    asm __volatile ("inb %w1,%b0" : "=a"(data) : "d"(port));
    return data;
}

uint16_t inw(uint16_t port)
{
    uint16_t data;
    asm volatile ("inw %w1,%w0" : "=a"(data) : "d"(port));
    return data;
}

uint32_t inl(uint16_t port) {
    uint32_t data;
    asm volatile ("inl %w1,%0" : "=a"(data) : "d"(port));
    return data;
}