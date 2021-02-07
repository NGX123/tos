/*
    @author = ngx123
    @brief = includes defines to use with the x86 CPUs
*/


#ifndef IO_X86_H
#define IO_X86_H


#include <stdint.h>


/*
    @brief = outputs 8 bits to the port
    @param port = the number of the port to output to
    @param data = the data to output to the port
*/
static void outb(uint16_t port, uint8_t data)
{
    asm volatile ("outb %b0,%w1" : : "a"(data), "d"(port));
}

/*
    @breif = writes 16 bits to the port
    @param port = the number of port to write to
    @param data = the data to output to the port
*/
static void outw(uint16_t port, uint16_t data)
{
  asm volatile ("outw %w0,%w1" : : "a"(data), "d"(port));
}

/*
    @breif = writes 32 bits to the port
    @param port = the number of port to write to
    @param data = the data to output to the port
*/
static void outl(uint16_t port, uint32_t data)
{
    asm volatile ("outl %0,%w1" : : "a"(data), "d"(port));
}


/*
    @brief = reads 8 bits from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
static uint8_t inb(uint16_t port)
{
     uint8_t data;
    asm __volatile ("inb %w1,%b0" : "=a"(data) : "d"(port));
    return data;
}

/*
    @brief = reads 16 bits from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
static uint16_t inw(uint16_t port)
{
    uint16_t data;
    asm volatile ("inw %w1,%w0" : "=a"(data) : "d"(port));
    return data;
}

/*
    @brief = reads 32 bits from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
static uint32_t inl(uint16_t port) {
    uint32_t data;
    asm volatile ("inl %w1,%0" : "=a"(data) : "d"(port));
    return data;
}
#endif