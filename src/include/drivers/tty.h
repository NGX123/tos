/*
    @author = ngx123
    @brief = global header for tty device driver
*/


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


/*
    @brief = writes bytes from the buffer to the screen
    @param buf = buffer to write from
    @param count = amount of bytes to write to the screen
    @return = amount of written bytes on success, -1 on fail
*/
extern ssize_t ttyWrite(void* buf, size_t count);

/*
    @brief = reads bytes form the keyboard to the buffer
    @param buf = buffer to read to
    @param count = amount of bytes to read from keyboard
    @return = amount of read bytes on success, -1 on fail
*/
extern ssize_t ttyRead(void* buf, size_t count);

/*
    @brief = controls device specific functions
    @param request = the number of cation that should be made
    @return = 0 or data on success, -1 on fial
*/
extern int ttyIoctl(size_t request, ...);
#endif