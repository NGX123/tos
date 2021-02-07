/*
    @author = ngx123
    @brief = vga device driver
*/


#include "include/vga.h"


static volatile char* text_buffer = (volatile char*)VGA_ADDRESS;
static enum VGA_COLOR terminal_fg;
static enum VGA_COLOR terminal_bg;
static uint8_t color;
static int cell;                                            // Counts cells(2 bytes - char + color)(last cell - 1999)
static int x;
static int y;


static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static void update_cursor(int x_axis, int y_axis)
{
	uint16_t pos = FORMULA_XY_CELL(x_axis, y_axis);

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static void updatexy()
{
    x = cell % 80;
    y = (int)(cell / 80);
    update_cursor(x, y);
}


static void backspace()
{
    --cell;

    text_buffer[FORMULA_CELL_CHARBYTE(cell)] = BLANK;
    updatexy();
}

static void enter()
{
    color = terminal_fg | terminal_bg << 4;

    ++y;
    x = 0;
    cell = FORMULA_XY_CELL(x, y);

    text_buffer[FORMULA_CELL_COLORBYTE(cell)] = (char)color;
    updatexy();
}

static void arrows(const char direction)
{
    if (direction == '<')
        --cell;
    else if (direction == '>')
        ++cell;
    updatexy();
}

static void cleanScreen(char cleanOption)
{
    int tmpCell;
    int tmpByte;

    if (cleanOption == CLEAN_SCREEN_CHAR_COLOR)
    {
        for (tmpByte = 0; tmpByte < 4000; tmpByte++)
            text_buffer[tmpByte] = BLANK;
    }

    if (cleanOption == CLEAN_SCREEN_CHAR)
    {
        for (tmpCell = 0; tmpCell < 2000; tmpCell++)
            text_buffer[FORMULA_CELL_CHARBYTE(tmpCell)] = BLANK;
    }

    if (cleanOption == CLEAN_SCREEN_COLOR)
    {
        for (tmpCell = 0; tmpCell < 2000; tmpCell++)
            text_buffer[FORMULA_CELL_COLORBYTE(tmpCell)] = BLANK;
    }
}

static void scrollScreen(){
    int currentCell;

    for (currentCell = 80; currentCell < 2000; currentCell++)
    {
        text_buffer[FORMULA_CELL_CHARBYTE((currentCell-80))] = text_buffer[currentCell * 2];
        text_buffer[FORMULA_CELL_COLORBYTE((currentCell-80))] = text_buffer[currentCell * 2 + 1];
    }

    for (currentCell = 1920; currentCell < 2000; currentCell++)
    {
        text_buffer[FORMULA_CELL_CHARBYTE(currentCell)] = BLANK;
        text_buffer[FORMULA_CELL_COLORBYTE(currentCell)] = color;
    }

    cell -= 80;
    updatexy();
}


int printScreen(const uint8_t character)
{
    int boundsCheckStatus = 0;

    // Make the text mode buffer bounds check
    if (cell >= 2000)
        boundsCheckStatus = -1;
    else if (cell <= 0)
        boundsCheckStatus = -2;


    if (character == '\n')
    {
        if (boundsCheckStatus == -1)
            return -1;

        // Scrolls if enter is pressed on last line
        if (cell >= 1920)
            scrollScreen();

        enter();

        return 0;
    }
    else if (character == '\t')
    {
        if (boundsCheckStatus == -1)
            return -1;

        // Scrolls if not enough space for tab on current line
        if (cell > (1999 - 4))
            scrollScreen();

        cell += 4;
        updatexy();

        return 0;
    }
    else if (character == LARROW)
    {
        if (boundsCheckStatus == -2)
            return -1;
        arrows('<');

        return 0;
    }
    else if (character == RARROW)
    {
        if (boundsCheckStatus == -1)
            return -1;

        // Does not move if on the last cell
        if (cell == 1999)
            return -1;

        arrows('>');

        return 0;
    }
    else if (character == '\b')
    {
        if (boundsCheckStatus == -2)
            return -1;
        backspace();

        return 0;
    }
    else
    {
        if (boundsCheckStatus == -1)
            return -1;

        // Scrolls if on the last cell
        if (cell == 1999)
            scrollScreen();

        text_buffer[FORMULA_CELL_CHARBYTE(cell)] = (char)character;
        text_buffer[FORMULA_CELL_COLORBYTE(cell)] = (char)color;

        ++cell;

        // Change the color of the cursor by changing next cell color
        text_buffer[FORMULA_CELL_COLORBYTE(cell)] = (char)color;
        updatexy();

        return 0;
    }

    return -1;
}


void initScreen(char cursor_status)
{
    terminal_fg = green;
    terminal_bg = black;
    color = terminal_fg | terminal_bg << 4;

    cleanScreen(CLEAN_SCREEN_CHAR_COLOR);

    changeColor(green, black, CHANGE_COLOR_ALL);

    if (cursor_status == CURSOR_ON)
        enable_cursor(0, 15);
    else if (cursor_status == CURSOR_OFF)
        disable_cursor();
}

void changeColor(enum VGA_COLOR fg, enum VGA_COLOR bg, int command)
{
    int tmpcell = 0;

    if (command == CHANGE_COLOR_NEXT)
    {
        terminal_fg = fg;
        terminal_bg = bg;
        color = terminal_fg | terminal_bg << 4;
    }

    else if (command == CHANGE_COLOR_ALL)
    {
        terminal_fg = fg;
        terminal_bg = bg;
        color = terminal_fg | terminal_bg << 4;

        while (tmpcell < 2000)
            text_buffer[tmpcell++ * 2 + 1] = (char)color;
    }

}


int vgatextWrite(void* buf, size_t count)
{
    size_t i;
    for (i = 0; i < count; i++)
        if (printScreen(((uint8_t*)buf)[i]) == -1)
            return -1;

    return 0;
}

int vgatextRead(void* buf, size_t count)
{
    int tempvar;
    tempvar = ((uint8_t*)buf)[count-1];

    return (tempvar * 0 - 1);
}