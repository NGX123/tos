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


// Initialises the screen
extern void initScreen(char cursorStatus);

// Changes the color of everything currently on the screen
extern void changeScreenColor(enum VGA_COLOR fg, enum VGA_COLOR bg);

// Changes the color of all the characters that will be printed next
extern void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg);

// Prints one character to the screen
extern void printScreen(const uint8_t character);
#endif