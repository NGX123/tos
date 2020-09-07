// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000



/// Includes ///
#include "drivers/x86.h"
#include "drivers/vga.h"

/// Defines ///
#define VGA 0xb8000
#define VGA_END 0xB8F9F
#define VGA_WIDTH 80
#define COLUMNS 80
#define ROWS 25  
#define BLANK 0

#define LARROW 0x80
#define RARROW 0x81


/// Declarations ///
// Colors for fore and background of text in vga text mode
static volatile char* text_buffer = (volatile char*)VGA;
static enum VGA_COLOR terminal_fg;
static enum VGA_COLOR terminal_bg;
static uint8_t color;
static int cell;                                            // Counts cells(2 bytes - char + color)(last cell - 1999)
static int byte;                                            // Counts each byte in video memory(last byte - 3999)
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
    
    text_buffer[byte] = BLANK;
    text_buffer[byte+1] = BLANK;

    --cell;
    byte = cell * 2;

    text_buffer[byte] = BLANK;
    text_buffer[byte+1] = BLANK;

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
    else if (direction == '>'){
        ++byte;
        ++byte;
        ++cell;
    }
    updatexy();
}

// Cleans the screen
static void cleanScreen(char cleanOption){
    int tmpCell;
    int tmpByte;

    // Clean colors and characters
    if (cleanOption == 0){
        for (tmpByte = 0; tmpByte < 4000; tmpByte++)
            text_buffer[tmpByte] = BLANK;
    }

    // Clean characters
    if (cleanOption == 1){
        for (tmpCell = 0; tmpCell < 2000; tmpCell++)
            text_buffer[tmpCell * 2] = BLANK;
    }

    // Clean color
    if (cleanOption == 2){
        for (tmpCell = 0; tmpCell < 2000; tmpCell++)
            text_buffer[tmpCell * 2 + 1] = BLANK;
    }
}




/// DRIVER INTERFACE ///
// Initialises the screen
void initScreen(char cursor_status){
    terminal_fg = green;
    terminal_bg = black;
    color = terminal_fg | terminal_bg << 4;
    
    // Clear screen
    cleanScreen(0);

    // Change the screen color
    changeScreenColor(green, black);

    // Change cursor status
    if (cursor_status == 1)
        enable_cursor(0, 15);
    else 
        disable_cursor();
}

// Changes color of all of the new text printed
void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg) {
    terminal_fg = fg;
    terminal_bg = bg;
    color = terminal_fg | terminal_bg << 4;
}

// Changes the color of all text
void changeScreenColor(enum VGA_COLOR fg, enum VGA_COLOR bg){
    terminal_fg = fg;
    terminal_bg = bg;
    color = terminal_fg | terminal_bg << 4;

    int tmpcell = 0;
    while (tmpcell < 2000)
        text_buffer[tmpcell++ * 2 + 1] = color; 
}

// Outputs a character to the screen
void printScreen(const uint8_t character){
    if (character == '\n')
        enter();
    else if (character == '\t')
        for (int i = 0; i <= 4; i++){
            text_buffer[byte++] = ' ';
            text_buffer[byte++] = color;

            ++cell;
        
            // Change the color of the cursor by changing next cell color
            text_buffer[cell * 2 + 1] = color; 
            updatexy();
        }  
    else if (character == LARROW)
        arrows('<');
    else if (character == RARROW){
        if (text_buffer[cell*2] != BLANK)
            arrows('>');
    }
    else if (character == '\b')
        backspace();
    else{
        text_buffer[byte++] = character;
        text_buffer[byte++] = color;

        ++cell;
        
        // Change the color of the cursor by changing next cell color
        text_buffer[cell * 2 + 1] = color; 
        updatexy();
    } 
}