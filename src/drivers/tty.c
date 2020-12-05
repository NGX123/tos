// File: tty.c
// Description: code for the tty device combining keyboard and display

#include "headers/tty.h"

static struct tty_config termios;
static ring_buffer_t ttyInputBufferStruct;
static uint8_t* ttyInputBuffer;
typedef int (*ioctlOps_t)(size_t, int*);

void ttyHandleKeyboardInterrupt(uint8_t character, uint8_t keyStatuses, uint32_t scancode){
    // Canonical/line mode
    if (termios.mode == MODE_CANON){
        // Skip the release events and the extra buttons events
        if (scancode < 0x80)
            return;

        // Printable ASCII Codes - Print to the screen
        if (character >= 32 && character <= 127){
            vgatextWrite(&character, 1);
            writeBuf(&ttyInputBufferStruct, character);
        }

        // Escape sequences - Print to the screen
        else if (character == '\b')
            vgatextWrite(&character, 1);

        // Handle the next line escape sequence as special case
        else if (character == '\n'){
            vgatextWrite(&character, 1);
            writeBuf(&ttyInputBufferStruct, character);
        }

        // Unimplemented
        else if (scancode == LARROW || scancode == RARROW || scancode == '\t')
            return;
    }

    else if (termios.mode == MODE_RAW){
        if (scancode < 0x80)
            return;

        // Characters
        if (character >= 32 && character <= 127)
            writeBuf(&ttyInputBufferStruct, character);

        // Escape codes
        else if (character == '\b' || character == '\t' || character == '\n')
            writeBuf(&ttyInputBufferStruct, character);

        // Arrows - implement escape sequences (e.g. ^[[C)
        else if (scancode == LARROW || scancode == RARROW){}
    }
}
static callroutine_t ttyKeyboardCallFunc = &ttyHandleKeyboardInterrupt;


/// IOCTL OPERATIONS ///
int ttyInit(uint8_t mode){
    RingBufferInit(&ttyInputBufferStruct, BUFFER_INPUT_SIZE, ttyInputBuffer);
    keyboardMode(KEYBOARD_MODE_STANDARD);
    keyboardCallFunc(ttyKeyboardCallFunc);
    termios.mode = mode;

    return 0;
}

/// FILE OPERATIONS ///
// Writes count from buf to screen
ssize_t ttyWrite(void* buf, size_t count){
    size_t i;

    for (i = 0; i < count; i++)
        if (writeBuf(&ttyInputBufferStruct, ((uint8_t*)buf)[count]) == -1){
            if (i == 0)
                return -1;
        }

    return (ssize_t)i;
}

// Reads count from keyboard to buf
ssize_t ttyRead(void* buf, size_t count){
    size_t i;
    int tmpVar;

    for (i = 0; i < count; i++)
        if((tmpVar = readBuf(&ttyInputBufferStruct)) == -1){
            if (i == 0)
                return -1;
        }
        else
            ((uint8_t*)buf)[i] = (uint8_t)tmpVar;

    return (ssize_t)i;
}