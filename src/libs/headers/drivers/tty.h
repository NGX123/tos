// File: tty.h
// Description: includes tty declraractions

#ifndef TTY_H
#define TTY_H

#include "types.h"

#define BUFFER_INPUT_SIZE 250
#define MODE_CANON        0
#define MODE_RAW          1

struct tty_config {
    uint8_t mode;
    uint8_t flags;
};

// Writes count from buf to screen
extern ssize_t ttyWrite(void* buf, size_t count);

// Reads count from keyboard to buf
extern ssize_t ttyRead(void* buf, size_t count);

// Controls special device specific functions
extern int ttyIoctl(size_t request, ...);

#endif