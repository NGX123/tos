// Outputting using x and y matrix
// Outputs a character
void printcxy(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character){
    uint8_t color = fg | bg << 4;
    
    cursor[y * COLUMNS + x] = character;
    updatexy();
    cursor[y * COLUMNS + x] = color;
    updatexy();
    update_cursor(x, y);
}

// Outputs a string
void printsxy(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    uint8_t color = fg | bg << 4;

    while (*string != 0){
        cursor[y * COLUMNS + x] = *string++;
        x++;
        cursor[y * COLUMNS + x] = color;
        x++;
    }
    update_cursor(x, y);
}

// Removes last letter
void backspacexy(){
    cursor[y * COLUMNS + x] = BLANK;
    x--;
    cursor[y * COLUMNS + x] = BLANK;
    x--;
    update_cursor(x, y);
}