/*
    @author = ngx123
    @breif = header for serial port driver
*/


#ifndef SERIAL_DEFINES_H
#define SERIAL_DEFINES_H


#include <arch/io_x86.h>


#define PORT 0x3f8


/*
    @brief = initialize the serial port device
*/
extern void initSerial(void);

/*
    @brief = send data through the serial port
    @param a = data to send
*/
extern void writeSerial(char* a);
#endif