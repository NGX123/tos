/*
    @author = ngx123
    @brief = PS/2 keyboard driver
*/


#include "include/kbd.h"


static uint8_t charBuffer[BUFSIZE];
static ring_buffer_t charRingBufferStruct;
static uint8_t kbd_mode = 0;
static callroutine_t callroutine = 0;


static int getButtonStatuses(uint32_t scancode, uint8_t* buttonStatuses){
    if ((scancode == 0x1D      || scancode == 0x1DE0) && !(*buttonStatuses & SHORTCUT_CTRL))
        *buttonStatuses ^= SHORTCUT_CTRL;
    else if ((scancode == 0x9D || scancode == 0x9DE0) &&  (*buttonStatuses & SHORTCUT_CTRL))
        *buttonStatuses ^= SHORTCUT_CTRL;

    if ((scancode == 0x38      || scancode == 0x38E0) && !(*buttonStatuses & SHORTCUT_ALT))
        *buttonStatuses ^= SHORTCUT_ALT;
    else if ((scancode == 0xB8 || scancode == 0xB8E0) &&  (*buttonStatuses & SHORTCUT_ALT))
        *buttonStatuses ^= SHORTCUT_ALT;

    if ((scancode == 0x2A      || scancode == 0x36)  && !(*buttonStatuses & SHORTCUT_SHIFT))
        *buttonStatuses ^= (SHORTCUT_SHIFT | KEYBYTE_CAPS);
    else if ((scancode == 0xAA || scancode == 0xB6)  &&  (*buttonStatuses & SHORTCUT_SHIFT))
        *buttonStatuses ^= (SHORTCUT_SHIFT | KEYBYTE_CAPS);

    if (scancode == 0x3A      && !(*buttonStatuses & SHORTCUT_CAPSLOCK))
        *buttonStatuses ^= (SHORTCUT_CAPSLOCK | KEYBYTE_CAPS);
    else if (scancode == 0x3A &&  (*buttonStatuses & SHORTCUT_CAPSLOCK))
        *buttonStatuses ^= (SHORTCUT_CAPSLOCK | KEYBYTE_CAPS);

    return 0;
}

static uint8_t getCharacter(uint32_t scancode, uint8_t buttonStatuses){
    uint8_t character = 0;

    if (!(buttonStatuses & (SHORTCUT_CTRL | SHORTCUT_ALT | KEYBYTE_CAPS)) && !(scancode & 0x80))
        character = scanset1[scancode];

    else if ((buttonStatuses & (KEYBYTE_CAPS)) && !(buttonStatuses & (SHORTCUT_CTRL | SHORTCUT_ALT)) && !(scancode & 0x80))
        character = shiftmap[scancode];

    // else if ((buttonStatuses & SHORTCUT_CTRL) && !(buttonStatuses & SHORTCUT_ALT))
    //     character = ctlmap[scancode];

    return character;
}


static void keyboardStdMode(uint32_t scancode, uint8_t character){
    if (character >= 32 && character <= 127 && scancode < 0x80)
        writeBuf(&charRingBufferStruct, character);

    else if (character == '\b' || character == '\t' || character == '\n')
        writeBuf(&charRingBufferStruct, character);

    else if (scancode == LARROW_SCAN)
        writeBuf(&charRingBufferStruct, LARROW);
    else if (scancode == RARROW_SCAN)
        writeBuf(&charRingBufferStruct, RARROW);
}

static void keyboardDisplayMode(uint32_t scancode, uint8_t character){
    if (character >= 32 && character <= 127 && scancode < 0x80)
        printScreen(character);

    if (character == '\b' || character == '\t' || character == '\n')
        printScreen(character);

    if (scancode == LARROW_SCAN)
        printScreen(LARROW);
    else if (scancode == RARROW_SCAN)
        printScreen(RARROW);
}


void keyboard_handler(){
    static uint8_t buttonStatuses = 0;                                                           // Toggle/Hold button statuses
    uint32_t keyboardStatus, scancode;                                                           // Data from ports
    uint8_t character, scancode_byte2;

    // Get the keyboard input
    if ((keyboardStatus = inb(KEYBOARD_STATUS_PORT)) & KBS_DIB)
        scancode = inb(KEYBOARD_DATA_PORT);
    else
        return;

    // Handle E0
    if (scancode == 0xE0){
        scancode_byte2 = inb(KEYBOARD_DATA_PORT);
        scancode = scancode | (scancode_byte2 << 8);
    }

    // Initialize the variables
    getButtonStatuses(scancode, &buttonStatuses);
    character = getCharacter(scancode, buttonStatuses);


    // Keyboard mode handling //
    if (kbd_mode == 0)
        keyboardStdMode(scancode, character);
    else if (kbd_mode == 1)
        keyboardDisplayMode(scancode, character);
    else if (kbd_mode == 2)
        return;
    if (callroutine != NULL){
        if (scancode != RARROW_SCAN && scancode != LARROW_SCAN)
            callroutine(character, scancode);
        else
            callroutine(scancode == RARROW_SCAN ? RARROW : LARROW, scancode);
    }
}


int keyboardInit(uint8_t mode){
    // Initialize the keyboard interrupts
    if (bindInterrupt(1, &keyboard_handler, INTERRUPT_PRIORITY_KERNEL) == -1)
        return -1;

    kbd_mode = mode;

    outb(KEYBOARD_STATUS_PORT, 0xAD);                           // Disable Second PS/2 Port
    outb(KEYBOARD_STATUS_PORT, 0xA7);                           // Disable Second PS/2 Port

    inb(KEYBOARD_DATA_PORT);                                    // Flush the input buffer

    outb(KEYBOARD_STATUS_PORT, 0xAA);
    while(!(inb(KEYBOARD_DATA_PORT) == 0x55));                  // Wait until test succesfully passed

    outb(KEYBOARD_STATUS_PORT, 0xA8);                           // Enable 2nd PS/2 port
    // NEED TO READ BYTE AND CHECK bit 5
    outb(KEYBOARD_STATUS_PORT, 0xA7);                           // Disable second PS/2 port

    RingBufferInit(&charRingBufferStruct, BUFSIZE, charBuffer);

    outb(KEYBOARD_COMMAND_PORT, 0xAE);                          // Enable 1st PS/2 port

    return 0;
}

int keyboardMode(int command){
    if (command == KEYBOARD_MODE_CURRENTMODE)
        return kbd_mode;

    if (command < 3){
        kbd_mode = command;
        return 0;
    }

    return -1;
}

int keyboardCallFunc(callroutine_t callroutine_func){
    callroutine = callroutine_func;
    return 0;
}


ssize_t keyboardRead(void* buf, size_t count){
    size_t i;
    int tmpVar;

    for (i = 0; i < count; i++)
        if((tmpVar = readBuf(&charRingBufferStruct)) == -1){
            if (i == 0)
                return -1;
        }
        else
            ((uint8_t*)buf)[i] = (uint8_t)tmpVar;

    return (ssize_t)i;
}

ssize_t keyboardWrite(void* buf, size_t count){
    int tempvar;
    tempvar = ((uint8_t*)buf)[count-1];

    return (tempvar * 0 - 1);
}