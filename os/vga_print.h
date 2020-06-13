// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000
// Problems: 



// Includes of platform independant librararies
#include <stdint.h>

// Defines
#define VGA 0xb8000


// Colors for fore and background of text in vga text mode
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



// Outputs the text and colors to the VGA text buffer
void write_string(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;


    // Pointer to VGA character buffer
    volatile char *video = (volatile char*)VGA;
    while(*string != 0){
        *video++ = *string++;
        *video++ = color;
    }
}