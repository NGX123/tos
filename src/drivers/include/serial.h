/*
    @author = ngx123
    @breif = header for serial port driver
*/


#ifndef SERIAL_DEFINES_H
#define SERIAL_DEFINES_H


#include <arch/io_x86.h>


#define PORT 0x3f8


/*
    @brief = check if the transmission line is empty before sending
    @return = status of transmission line
*/
static int is_transmit_empty();

/*
    @brief = initialize the serial port device
*/
extern void initSerial();

/*
    @brief = send data through the serial port
    @param a = data to send
*/
extern void writeSerial(uint8_t* a);
#endif