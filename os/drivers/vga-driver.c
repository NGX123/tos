// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000
// Problems: 



#include <stdint.h>
#include "drivers.h"
#define VGA 0xb8000

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

// Outputs a character
void write_char(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;

    volatile char *video = (volatile char*)VGA;
    *video++ = character;
    *video = color;
}