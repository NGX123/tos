// File: vga.h
// Description: defines for the vga driver

#ifndef VGA_DEFINES_H
#define VGA_DEFINES_H

/// Includes
#include "drivers/x86.h"
#include "drivers/vga.h"
#include "drivers/kbd.h"

/// Defines
#define VGA_ADDRESS 0xb8000
#define VGA_END 0xB8F9F
#define COLUMNS 80
#define ROWS 25
#define BLANK 0

#define FORMULA_XY_CELL(x_pos, y_pos) (y_pos * COLUMNS + x_pos)
#define FORMULA_CELL_CHARBYTE(currentCell) (currentCell * 2)
#define FORMULA_CELL_COLORBYTE(currentCell) (currentCell * 2 + 1)



/// Declarations
// Enable the cursor
static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

// Disable the cursor
static void disable_cursor();

// Update the position of the cursor
static void update_cursor(int x_axis, int y_axis);

// Updates the cursor location
static void updatexy();


// Removes last printed letter
static void backspace();

// Moves onto the next line
static void enter();

// Moves cursor accrording to keyboard arrows
static void arrows(const char direction);

// Cleans the screen
static void cleanScreen(char cleanOption);

// Scrolls the screen
void scrollScreen();

// Outputs a character to the screen
int printScreen(const uint8_t character);

// Initialises the screen
void initScreen(char cursor_status);

// Changes color of all of the new text printed
void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg, int command);

// Writes from buf to display
int vgatextWrite(void* buf, size_t count);

// Reads from display to buf
int vgatextRead(void* buf, size_t count);
#endif