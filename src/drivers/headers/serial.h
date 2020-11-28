// File: serial.h
// Description: defines for the serial-port driver

#ifndef SERIAL_DEFINES_H
#define SERIAL_DEFINES_H

/// Includes
#include "drivers/x86.h"

#define PORT 0x3f8

/// Declarations
// Initialize the serial port configuration
void initSerial();

// Check if the port is empty and ready for transmission
int is_transmit_empty();

// Send the data through serial port
void writeSerial(uint8_t* a);
#endif