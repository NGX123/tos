// File: tty.h
// Description: defines for the tty driver

#ifndef TTY_DEFINES_H
#define TTY_DEFINES_H

/// Includes
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "drivers/vga.h"
#include "drivers/kbd.h"
#include "drivers/tty.h"
#include "ringbuf.h"
#include "types.h"

/// Declarations
// Handles keyboard interrupts
void ttyHandleKeyboardInterrupt(uint8_t character, uint8_t keyStatuses, uint32_t scancode);

// Initializes the tty0
int ttyInit(uint8_t mode);

// Writes count from buf to screen
ssize_t ttyWrite(void* buf, size_t count);

// Reads count from keyboard to buf
ssize_t ttyRead(void* buf, size_t count);

// Calls device specific functions
int ttyIoctl(size_t request, ...);
#endif