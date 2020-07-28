// Outputs the text and colors to the VGA text buffer
void prints(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    uint8_t color = fg | bg << 4;

    while(*string != 0){
        text_buffer[byte++] = *string++;
        text_buffer[byte++] = color;
        
        ++cell;
    }
    
    text_buffer[cell * 2 + 1] = color;
    updatexy();
}