// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000



/// Includes ///
#include "drivers/x86.h"
#include "drivers/vga.h"
#include "drivers/kbd.h"

/// Defines ///
#define VGA_ADDRESS 0xb8000
#define VGA_END 0xB8F9F
#define COLUMNS 80
#define ROWS 25  
#define BLANK 0

#define FORMULA_XY_CELL(x_pos, y_pos) (y_pos * COLUMNS + x_pos)
#define FORMULA_CELL_CHARBYTE(currentCell) (currentCell * 2)
#define FORMULA_CELL_COLORBYTE(currentCell) (currentCell * 2 + 1)

/// Declarations ///
static volatile char* text_buffer = (volatile char*)VGA_ADDRESS;
static enum VGA_COLOR terminal_fg;
static enum VGA_COLOR terminal_bg;
static uint8_t color;
static int cell;                                            // Counts cells(2 bytes - char + color)(last cell - 1999)
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
	uint16_t pos = FORMULA_XY_CELL(x, y);
 
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
    --cell;

    text_buffer[FORMULA_CELL_CHARBYTE(cell)] = BLANK;
    updatexy();
}

// Moves onto the next line
static void enter(){
    color = terminal_fg | terminal_bg << 4;

    ++y;
    x = 0;
    cell = FORMULA_XY_CELL(x, y);

    text_buffer[FORMULA_CELL_COLORBYTE(cell)] = color;
    updatexy();
}

// Moves cursor accrording to keyboard arrows 
static void arrows(const char direction){
    if (direction == '<')
        --cell;
    else if (direction == '>')
        ++cell;
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
            text_buffer[FORMULA_CELL_CHARBYTE(tmpCell)] = BLANK;
    }

    // Clean color
    if (cleanOption == 2){
        for (tmpCell = 0; tmpCell < 2000; tmpCell++)
            text_buffer[FORMULA_CELL_COLORBYTE(tmpCell)] = BLANK;
    }
}

// Scrolls the screen 
void scrollScreen(){
    int currentCell;
    
    for (currentCell = 80; currentCell < 2000; currentCell++)
        text_buffer[(currentCell-80) * 2] = text_buffer[currentCell * 2];

    for (currentCell = 1920; currentCell < 2000; currentCell++)
        text_buffer[currentCell * 2] = BLANK;

    cell -= 80;
    updatexy();
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
    changeColor(green, black, CHANGE_COLOR_ALL);

    // Change cursor status
    if (cursor_status == CURSOR_ON)
        enable_cursor(0, 15);
    else if (cursor_status == CURSOR_OFF)
        disable_cursor();
}

// Changes color of all of the new text printed
void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg, int command) {
    int tmpcell = 0;

    // Change color of all text printed next
    if (command == CHANGE_COLOR_NEXT){
        terminal_fg = fg;
        terminal_bg = bg;
        color = terminal_fg | terminal_bg << 4;
    }

    // Change color of all of next and current text on the screen
    else if (command == CHANGE_COLOR_ALL) {
        terminal_fg = fg;
        terminal_bg = bg;
        color = terminal_fg | terminal_bg << 4;

        while (tmpcell < 2000)
            text_buffer[tmpcell++ * 2 + 1] = color; 
    }
    
}

// Sets a marker which is the start and end of current write space; size is the size of buffer
int videoBuffer(int command, int size){
    static int bufferStatus = 0;
    static int videoBufferStart = 0;
    static int videoBufferEnd = 0;

    // Turn on the buffering of everything written - and any input, arrows... can't go beyond the buffer
    if (command == BUFFER_ON){
        videoBufferStart = cell;
        videoBufferEnd = cell + size;

        if (size == BUFFER_SIZE_SCREEN_END)
            videoBufferEnd = 2000;
        
        // Buffer can't be more then available space
        if (videoBufferEnd > 2000){
            videoBufferStart = 0;
            videoBufferStart = 0;
            videoBufferEnd = 0;

            return -1;
        }

        bufferStatus = 1;

        return bufferStatus;
    }

    // Turn off the buffer
    else if (command == BUFFER_OFF){
        bufferStatus = 0;
        videoBufferStart = 0;
        videoBufferEnd = 0;

        return bufferStatus;
    }

    // Check if we came to the end of the buffer
    else if (command == BUFFER_CHECK) {
        if (cell >= videoBufferEnd)
            return -1;
        else if (cell <= videoBufferStart)
            return -2;
        else if (bufferStatus == 0)
            return -3;

    }

    // Check the buffer status
    else if (command == BUFFER_STATUS){
        return bufferStatus;
    }
        
    return -4;
}

// Outputs a character to the screen
int printScreen(const uint8_t character){
    // Check the text on bound
    int boundsCheckStatus = 0;
    if (videoBuffer(BUFFER_STATUS, 0) == 0)
        boundsCheckStatus = 0;
    else if (videoBuffer(BUFFER_STATUS, 0) == 1)
        boundsCheckStatus = videoBuffer(BUFFER_CHECK, 0);
    
    // Make the text mode buffer bounds check
    if (cell >= 2000)
        boundsCheckStatus = -1;
    else if (cell <= 0)
        boundsCheckStatus = -2;


    // HANDLE ASCII //
    // Handle next line character
    if (character == '\n'){
        if (boundsCheckStatus == -1)
            return -1;

        // Scrolls if enter is pressed on last line
        if (cell >= 1920)
            scrollScreen();

        enter();

        return 0;
    }

    // Handle tab 
    else if (character == '\t'){
        if (boundsCheckStatus == -1)
            return -1;

        // Scrolls if not enough space for tab on current line
        if (cell > (1999 - 4))
            scrollScreen();

        cell += 4;
        updatexy();

        return 0;
    }  
        
    // Handle arrows
    else if (character == LARROW){
        if (boundsCheckStatus == -2)
            return -1;
        arrows('<');

        return 0;
    }
    else if (character == RARROW){
        if (boundsCheckStatus == -1)
            return -1;

        // Does not move if on the last cell
        if (cell == 1999)
            return -1;

        arrows('>');

        return 0;
    }

    // Handle backspace
    else if (character == '\b'){
        if (boundsCheckStatus == -2)
            return -1;
        backspace();

        return 0;
    }

    // Handle letter and symbol ASCII
    else {
        if (boundsCheckStatus == -1)
            return -1;
        
        // Scrolls if on the last cell
        if (cell == 1999)
            scrollScreen();

        text_buffer[FORMULA_CELL_CHARBYTE(cell)] = character;
        text_buffer[FORMULA_CELL_COLORBYTE(cell)] = color;

        ++cell;
        
        // Change the color of the cursor by changing next cell color
        text_buffer[FORMULA_CELL_COLORBYTE(cell)] = color; 
        updatexy();
        
        return 0;
    } 

    return -1;
}