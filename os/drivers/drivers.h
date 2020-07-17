// File: drivers.h
// Description: incldes declaration for the driver functions

#include <stdint.h>
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
char* current_vga;

void write_char(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character);
void write_string(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string);

void init_serial();
void write_serial(char* a);

void setFlat();
