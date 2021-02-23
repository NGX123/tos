/*
    @author = ngx123
    @brief = local header for tty device driver
*/


#ifndef TTY_DEFINES_H
#define TTY_DEFINES_H


#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <drivers/vga.h>
#include <drivers/kbd.h>
#include <drivers/tty.h>
#include <ringbuf.h>


/*
    @brief = handles the calls from the keyboard interrupt handler
    @param character = the character that should be interprted
    @param scancode = scancode that should help interpret the character
*/
static void ttyHandleKeyboardInterrupt(uint8_t character, uint32_t scancode);
#endif