/*
    @author = ngx123
    @brief = local header for vga driver
*/


#ifndef VGA_DEFINES_H
#define VGA_DEFINES_H


#include <arch/io_x86.h>
#include <drivers/vga.h>
#include <drivers/kbd.h>


#define VGA_ADDRESS 0xb8000
#define VGA_END 0xB8F9F
#define COLUMNS 80
#define ROWS 25
#define BLANK 0

#define FORMULA_XY_CELL(x_pos, y_pos) (y_pos * COLUMNS + x_pos)
#define FORMULA_CELL_CHARBYTE(currentCell) (currentCell * 2)
#define FORMULA_CELL_COLORBYTE(currentCell) (currentCell * 2 + 1)

#define CLEAN_SCREEN_CHAR_COLOR 0
#define CLEAN_SCREEN_CHAR 1
#define CLEAN_SCREEN_COLOR 2


/*
    @brief = enables the cursor
    @param cursor_start = the bottom limit of cursor size
    @param cursor_end = the upper limit of the cursor size
*/
static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

/*
    @brief = disables the cursor
*/
static void disable_cursor();

/*
    @brief = updates the position of the cursor
    @param x_axis = the x axis where the cursor should be sent
    @param y_axis = the y axis where the cursor should be sent
*/
static void update_cursor(int x_axis, int y_axis);

/*
    @brief = updates the xy coordinates relying on global variables that are changed by other functions
*/
static void updatexy();

/*
    @brief = removes the last printed letter from the screen
*/
static void backspace();

/*
    @brief = moves the cursor on to the next line
*/
static void enter();

/*
    @brief = changes the location of the cursor based on the arrows pressed
    @param direction = specifies arrow pointing in which direction was pressed
*/
static void arrows(const char direction);

/*
    @brief = cleans the screen based on the supplied parameters
    @param cleanOption = the mode in which the screen should be cleaned
*/
static void cleanScreen(char cleanOption);
#endif