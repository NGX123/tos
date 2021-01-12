/*
    @author = ngx123
    @brief = 8042 PS/2 controller driver
*/


#include "include/ps2_8042.h"
#include "stdio.h"


static struct controller_info recieved_data;


/*
    @brief = initialization of the PS/2 controller
    @return = 0 on success, -1 on error
*/
int ps2ControllerInit()
{
    uint8_t config_byte;

    // Disable USB Legacy

    // Check if the controller exists with ACPI

    // Disable ports so they don't interrupt
    outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT1);
    outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT2);

    // Flush the data port
    inb(CONTROLLER_DATA_PORT);

    // Read and Edit the config byte
    outb(CONTROLLER_COMMAND_PORT, READ_CONFIG_BYTE);
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
    {
        config_byte = inb(CONTROLLER_DATA_PORT);
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
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
    {
        if (!(inb(CONTROLLER_DATA_PORT) & CONFIG_PS2_PORT2_CLOCK) && (config_byte & CONFIG_PS2_PORT2_CLOCK))
        {
            recieved_data.channels_present = 2;
            outb(CONTROLLER_COMMAND_PORT, DISABLE_PS2_PORT2);
        }
        else
            recieved_data.channels_present = 1;
    }
    else
        return -1;

    // Perform interface self-tests and if none succeed return -1
    outb(CONTROLLER_COMMAND_PORT, TEST_PS2_PORT1);                  // Port 1
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
        if (inb(CONTROLLER_DATA_PORT) == 0x00)
            recieved_data.channel1_status = CHANNEL_STATUS_WORKING;
    if (recieved_data.channels_present == 2)
    {
        outb(CONTROLLER_COMMAND_PORT, TEST_PS2_PORT2);              // Port 2
        if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
            if (inb(CONTROLLER_DATA_PORT) != 0x00)
                recieved_data.channel2_status = CHANNEL_STATUS_WORKING;
    }
    // Fail if all existing channels are broken
    if ((recieved_data.channels_present == 1 && recieved_data.channel1_status == CHANNEL_STATUS_BROKEN) || (recieved_data.channels_present == 2 && (recieved_data.channel1_status == CHANNEL_STATUS_BROKEN && recieved_data.channel2_status == CHANNEL_STATUS_BROKEN)))
        return -1;

    // Enable the ports
    outb(CONTROLLER_COMMAND_PORT, ENABLE_PS2_PORT1);
    if (recieved_data.channels_present == 2)
        outb(CONTROLLER_COMMAND_PORT, ENABLE_PS2_PORT2);

    // Reset the connected devices
    if (recieved_data.channel1_status == CHANNEL_STATUS_WORKING)    // Device on Channel 1
    {
        inb(CONTROLLER_DATA_PORT);                                  // Flush data port
        if (!(inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER))
        {
            outb(CONTROLLER_DATA_PORT, 0xFF);
            if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
            {
                if (inb(CONTROLLER_DATA_PORT) == 0xFA)
                    //printf("SUCCESS\n")
                    ;
                else
                    recieved_data.channel1_device = DEVICE_BROKEN;
            }
        }
        else
            return -1;
    }
    if (recieved_data.channels_present == 2 && recieved_data.channel2_status == CHANNEL_STATUS_WORKING)
    {                                                               // Device on Channel 2
        outb(CONTROLLER_COMMAND_PORT, WRITE_PS2_PORT2_INPUT);
        if (!(inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER))
        {
            outb(CONTROLLER_DATA_PORT, 0xFF);
            if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
            {
                if (inb(CONTROLLER_DATA_PORT) == 0xFA)
                    //printf("SUCCESS 2\n")
                    ;
                else
                    recieved_data.channel2_device = DEVICE_BROKEN;
            }
        }
        else
            return -1;
    }

    // printf("%d %d %d %d %d\n", recieved_data.channel1_device, recieved_data.channel1_status, recieved_data.channel2_device, recieved_data.channel2_status, recieved_data.channels_present);

    return 0;
}


// static int ioctl_requestData(uint8_t datatype, uint8_t* data)
// {
//     return 0;
// }

static int ioctl_controllerDataRecieve(uint8_t command, uint8_t data)
{
    outb(CONTROLLER_COMMAND_PORT, command);
    if (inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER)
        data = inb(CONTROLLER_DATA_PORT);
    else
        return -1;

    return 0;
}

static int ioctl_controllerDataSend(uint8_t command, uint8_t data)
{
    outb(CONTROLLER_COMMAND_PORT, command);
    if (!(inb(CONTROLLER_STATUS_PORT) & STATUS_OUTPUT_BUFFER))
        outb(CONTROLLER_DATA_PORT, data);
    else
        return -1;

    return 0;
}

static int ioctl_controllerDirectSend(uint16_t port, uint8_t data)
{
    if (port == CONTROLLER_DATA_PORT || port == CONTROLLER_STATUS_PORT || port == CONTROLLER_COMMAND_PORT)
        outb(port, data);
    else
        return -1;

    return 0;
}

static int ioctl_controllerDirectRecieve(uint16_t port, uint8_t* data)
{
    if (port == CONTROLLER_DATA_PORT || port == CONTROLLER_STATUS_PORT || port == CONTROLLER_COMMAND_PORT)
        *data = inb(port);
    else
        return -1;

    return 0;
}

int ioctl(unsigned long request, ...)
{
    va_list valist;
    if (request <= IOCTL_CONTROLLER_RECEIVE)
        va_start(valist, request);

    if (request == IOCTL_CONTROLLER_SEND)
        return ioctl_controllerDataSend(0, 0); // TO FINISH
    else if (request == IOCTL_CONTROLLER_RECEIVE)
        return ioctl_controllerDataRecieve(0, 0); // TO FINISH
    else if (request == IOCTL_CONTROLLER_DIRECT_SEND)
        return ioctl_controllerDirectSend(0, 0); // TO FINISH
    else if (request == IOCTL_CONTROLLER_DIRECT_RECIEVE)
        return ioctl_controllerDirectRecieve(0, 0); // TO FINISH

    va_end(valist);
    return 0;
}


// ssize_t ps2_8042Write(void* buf, size_t count)
// {
//     return -1;
// }

// ssize_t ps2_8042Read(void* buf, size_t count)
// {
//     return -1;
// }