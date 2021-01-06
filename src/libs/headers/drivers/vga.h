/*
    @author = ngx123
    @brief = global header for vga driver
*/


#ifndef VGA_H
#define VGA_H


#include <stdint.h>
#include <stddef.h>


#define CHANGE_COLOR_NEXT 0
#define CHANGE_COLOR_ALL 1

#define CURSOR_OFF 0
#define CURSOR_ON 1


// Colors for the VGA 16-bit text mode
enum VGA_COLOR {
    black = 0x0,
    blue = 0x1,
    green = 0x2,
    cyan = 0x3,
    red = 0x4,
    purple = 0x5,
    brown = 0x6,
    gray = 0x7,
    dark_grey = 0x8,
    light_blue = 0x9,
    light_green = 0xA,
    light_cyan = 0xB,
    light_red = 0xC,
    light_purple = 0xD,
    yellow = 0xE,
    white = 0xF
};


/*
    @brief = initializes the screen
    @param cursorStatus = specifies if the cursor is off or on
*/
extern void initScreen(char cursorStatus);

/*
    @brief = changes the color of all next characters or all characters according to the mode
    @param fg = foreground color of the characters
    @param bg = background color of the character
    @param command = specifies which characters should be changed in color - all or only all printed after function was called
*/
extern void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg, int command);

/*
    @brief = prints one character to the screen
    @param character = the character that should be printed
    @return = 0 on success, -1 on fail
*/
extern int printScreen(const uint8_t character);

/*
    @brief = writes to bytes from buffer to the screen
    @param buf = the buffer to write bytes from
    @param count = the amount of bytes to write from the buffer
    @return = amount of bytes written on success, -1 on fail
*/
extern int vgatextWrite(void* buf, size_t count);

/*
    @brief = reads bytes from the vga buffer
    @param buf = the buffer to read bytes to
    @param count = amount of bytes to read to the buffer
    @return = amount of bytes read on success, -1 on fail
*/
extern int vgatextRead(void* buf, size_t count);
#endif