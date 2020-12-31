// File: vga.h
// Description: includes defenition of vga control functions

#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

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

#define CHANGE_COLOR_NEXT 0
#define CHANGE_COLOR_ALL 1

#define CURSOR_OFF 0
#define CURSOR_ON 1

// Initialises the screen
extern void initScreen(char cursorStatus);

// Changes the color of all the characters that will be printed next
extern void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg, int command);

// Prints one character to the screen
extern int printScreen(const uint8_t character);

// Writes count from buf to screen
extern int vgatextWrite(void* buf, size_t count);

// Reads count from screen to buf
extern int vgatextRead(void* buf, size_t count);
#endif