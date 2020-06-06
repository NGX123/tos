// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000


#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Defines
#define VGA 0xb8000

enum VGAC {
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    purple = 5,
    brown = 6,
    gray = 7,
    dark_grey = 8,
    light_blue = 9,
    light_green = 10,
    light_cyan = 11,
    light_red = 12,
    light_purple = 13,
    yellow = 14,
    white = 15
};

enum VGAC clr;

void write_string(int colour, const char *string){
    // Pointer to VGA character buffer
    volatile char *video = (volatile char*)VGA;
    while(*string != 0){
        *video++ = *string++;
        *video++ = colour;
    }
}