// Libraries that depend on linux could not be used
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


//Check if the compiler does not target unix
#if defined(__linux__)
#error "Cross-comipler is not used"
#endif

#if !defined(__i386__)
#error "Wrong compiler architecture"
#endif

enum vga_color {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};


// Returns the bit cmombination for writing to vga
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

// Combines the byte with attributes and the ASCII byte
static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
    return (uint16_t)uc | (uint16_t)color << 8;
};

// Returns the length of the string
size_t strlen(const char* str){
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

// Declare variables and constants
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;


void terminal_initialize(void){
    
}
