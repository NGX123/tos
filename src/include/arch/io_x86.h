/*
    @author = ngx123
    @brief = includes declarations for port IO on x86
*/


#ifndef IO_X86_H
#define IO_X86_H


#include <stdint.h>


/*
    @brief = outputs 8 bits to the port
    @param port = the number of the port to output to
    @param data = the data to output to the port
*/
void outb(uint16_t port, uint8_t data);

/*
    @breif = writes 16 bits to the port
    @param port = the number of port to write to
    @param data = the data to output to the port
*/
void outw(uint16_t port, uint16_t data);

/*
    @breif = writes 32 bits to the port
    @param port = the number of port to write to
    @param data = the data to output to the port
*/
void outl(uint16_t port, uint32_t data);


/*
    @brief = reads 8 bits from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
uint8_t inb(uint16_t port);

/*
    @brief = reads 16 bits from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
uint16_t inw(uint16_t port);

/*
    @brief = reads 32 bits from the port
    @param port = the number of the port to read from
    @return = the data read from the port
*/
uint32_t inl(uint16_t port);
#endif