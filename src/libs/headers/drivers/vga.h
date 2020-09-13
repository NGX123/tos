// File: vga.h
// Description: includes defenition of vga control functions



#ifndef VGA_H
#define VGA_H
#include <stdint.h>

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

#define BUFFER_ON 0
#define BUFFER_OFF 1
#define BUFFER_CHECK 2
#define BUFFER_STATUS 3
#define BUFFER_SIZE_SCREEN_END 2000

#define CHANGE_COLOR_NEXT 0
#define CHANGE_COLOR_ALL 1

// Initialises the screen
extern void initScreen(char cursorStatus);

// Changes the color of all the characters that will be printed next
extern void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg, int command);

// Prints one character to the screen
extern void printScreen(const uint8_t character);

// Controls VGA Video Buffer
extern int videoBuffer(int command, int size);
#endif