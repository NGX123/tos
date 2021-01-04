// File: 8042_ps2.c
// Description: a device driver for the 8042 PS/2 controller

#include "headers/ps2_8042.h"
#include "stdio.h"

int printBin(int num)
{
  int c, k;

  for (c = 31; c >= 0; c--)
  {
    k = num >> c;

    if (k & 1)
      printf("1");
    else
      printf("0");
  }

  printf("\n");

  return 0;
}


int ps2ControllerInit(){
    uint8_t test;
    uint8_t config_byte;
    int channels;
    int channel1_status = CHANNEL_STATUS_BROKEN;
    int channel2_status = CHANNEL_STATUS_BROKEN;

    // Disable USB Legacy

    // Check if the controller exists with ACPI

    // Disable ports so they don't interrupt
    outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT1);
    outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT2);

    // Flush the data port
    inb(CONTROLLER_DATA_PORT);

    // Read and Edit the config byte
    outb(CONTROLLER_COMMAND_PORT, READ_CONFIG_BYTE);
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER){
        config_byte = inb(CONTROLLER_DATA_PORT);
        printf("Byte - %x\n", config_byte);
        printf("Post - %d\n", (config_byte & CONFIG_POST_PASSED));
    }
    else
        return -1;

    // Perform a controller self-test and if the self test does not return success(0x55) return -1
    outb(CONTROLLER_COMMAND_PORT, TEST_PS2_CONTROLLER);
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
        if (inb(CONTROLLER_DATA_PORT) != 0x55)
            return -1;

    // Check if the controller is single or dual channel
    outb(CONTROLLER_COMMAND_PORT, ENABLE_PS2_PORT2);
    outb(CONTROLLER_COMMAND_PORT, READ_CONFIG_BYTE);
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER){
        if (!(inb(CONTROLLER_DATA_PORT) & CONFIG_PS2_PORT2_CLOCK) && (config_byte & CONFIG_PS2_PORT2_CLOCK)){
            channels = 2;
            outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT2);
        }
        else
            channels = 1;
    }
    else
        return -1;

    // Perform interface self-tests and if none succeed return -1
    outb(CONTROLLER_COMMAND_PORT, TEST_PS2_PORT1);                      // Port 1
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
        if (inb(CONTROLLER_DATA_PORT) == 0x00)
            channel1_status = CHANNEL_STATUS_WORKING;
    if (channels == 2){
        outb(CONTROLLER_COMMAND_PORT, TEST_PS2_PORT2);                  // Port 2
        if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
            if (inb(CONTROLLER_DATA_PORT) != 0x00)
                channel2_status = CHANNEL_STATUS_WORKING;
    }
    // Fail if all existing channels are broken
    if ((channels == 1 && channel1_status == CHANNEL_STATUS_BROKEN) || (channels == 2 && (channel1_status == CHANNEL_STATUS_BROKEN && channel2_status == CHANNEL_STATUS_BROKEN)))
        return -1;

    // Enable the ports
    outb(CONTROLLER_COMMAND_PORT, ENABLE_PS2_PORT1);
    if (channels == 2)
        outb(CONTROLLER_COMMAND_PORT, ENABLE_PS2_PORT2);

    // Reset the connected devices

    printf("Channels amount - %d\n", channels);

    return 0;
}