/*
    @author = ngx123
    @brief = local header for tty device driver
*/


#ifndef TTY_DEFINES_H
#define TTY_DEFINES_H


#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "drivers/vga.h"
#include "drivers/kbd.h"
#include "drivers/tty.h"
#include "ringbuf.h"
#include "types.h"


/*
    @brief = handles the calls from the keyboard interrupt handler
    @param character = the character that should be interprted
    @param scancode = scancode that should help interpret the character
*/
static void ttyHandleKeyboardInterrupt(uint8_t character, uint32_t scancode);

/*
    @biref = initializes the tty driver
    @param mode = the mode in which the tty driver should work
    @return = 0 on success
*/
static int ttyInit(uint8_t mode);
#endif