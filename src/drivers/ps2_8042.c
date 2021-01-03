// File: 8042_ps2.c
// Description: a device driver for the 8042 PS/2 controller

#include "headers/ps2_8042.h"

int ps2ControllerInit(){
    // uint8_t config_byte;

    // // Disable ports so they don't interrupt
    // outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT1);
    // outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT2);

    // // Change the config byte
    // outb(CONTROLLER_COMMAND_PORT, READ_CONFIG_BYTE);
    // if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
    //     printf("%x", inb(CONTROLLER_DATA_PORT));

    outb(CONTROLLER_COMMAND_PORT, TEST_PS2_PORT1);
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
        printf("%d\n", inb(CONTROLLER_DATA_PORT));

    return 0;
}