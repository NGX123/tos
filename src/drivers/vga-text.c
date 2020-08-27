// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000



/// Includes ///
#include "x86.h"
#include "driver.h"

/// Defines ///
#define VGA 0xb8000
#define VGA_WIDTH 80
#define COLUMNS 80
#define ROWS 25  
#define BLANK 0




/// Declarations ///
// Colors for fore and background of text in vga text mode
static volatile char* text_buffer = (volatile char*)VGA;
static enum VGA_COLOR terminal_fg;
static enum VGA_COLOR terminal_bg;
static uint8_t color;
static int cell; // Counts cells(2 bytes - char + color)
static int byte; // Counts each byte in video memory
static int x;
static int y;




/// CURSOR ///
// Enable the cursor
static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// Disable the cursor 
static void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

// Update the position of the cursor 
static void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static void updatexy(){
    x = cell % 80; 
    y = (int)(cell / 80);
    update_cursor(x, y);
}




//// SPECIAL SYMBOLS ///
// Removes last printed letter
static void backspace(){
    uint8_t color = terminal_fg | terminal_bg << 4;
    
    --byte;
    --byte;
    text_buffer[byte] = BLANK;

    --cell;
    text_buffer[cell * 2 + 1] = color; 
    updatexy();
}

// Moves onto the next line
static void enter(){
    uint8_t color = terminal_fg | terminal_bg << 4;

    ++y;
    x = 0;
    cell = y * 80 + x;
    byte = cell * 2;

    text_buffer[cell * 2 + 1] = color;
    updatexy();
}

// Moves cursor accrording to keyboard arrows 
static void arrows(const char direction){
    if (direction == '<'){
        --byte;
        --byte;
        --cell;
    }
    else if (text_buffer[cell * 2] != 0){
        ++byte;
        ++byte;
        ++cell;
    }
    updatexy();
}

// Clears the screen and moves the cursor to the start
static void clear(){
    volatile char* text_buffer = (char*)VGA;
    for (int i = 0; i <= 4000; i++){
        text_buffer[i] = BLANK;
    }
    byte = 0;
    cell = 0;
    updatexy();
}




/// DRIVER INTERFACE ///
// Initialises the screen
void initScreen(){
    terminal_fg = green;
    terminal_fg = black;
    color = terminal_fg | terminal_bg << 4;

    enable_cursor(0, 15);
}

// Changes screen color 
void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg) {
    terminal_fg = fg;
    terminal_bg = bg;
    color = terminal_fg | terminal_bg << 4;
}

// Outputs a character to the screen
void printScreen(const char character){
    if (character == '\n')
        enter();
    else if (character == '\t')
        for (int i = 0; i <= 4; i++)
            printc(green, black, ' ');
    else{
        text_buffer[byte++] = character;
        text_buffer[byte++] = color;

        ++cell;
        
        // Change the color of the cursor by changing next cell color
        text_buffer[cell * 2 + 1] = color; 
        updatexy();
    } 
}