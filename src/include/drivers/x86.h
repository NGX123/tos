/*
    @author = ngx123
    @brief = includes defines to use with the x86 CPUs
*/


#ifndef X86_H
#define X86_H


#include <stdint.h>


/*
    @brief = reads one byte from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
static inline uint8_t inb(uint16_t port)
{
    uint8_t data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

/*
    @brief = outputs one byte to the port
    @param port = the number of the port to output to
    @param data = the data to output to the port
*/
static inline void outb(uint16_t port, uint8_t data)
{
    asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

/*
    @breif = reads a words from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
static inline uint16_t inw(uint16_t port)
{
    uint16_t data;

    asm volatile("in %1,%0" : "=a" (data) : "d" (port));
    return data;
}

/*
    @breif = writes word to the port
    @param port = the number of port to write to
    @param data = the data to output to the port
*/
static inline void outw(uint16_t port, uint16_t data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}
#endif
