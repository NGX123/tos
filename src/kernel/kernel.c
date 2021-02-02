/*
    @author = ngx123
    @brief = main file from which function in others are started
*/


#include "include/kernel.h"


void kernel_main()
{
    // Turns the cursor on and sets it size
    initScreen(CURSOR_ON);
    printsys("VGA Text Mode\n", PRINTSYS_STATUS_SUCCESS);


    // Initializes the platform specific stuff
    if (hardwarePlatformInit() != -1)
        printsys("Platform\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Platform\n", PRINTSYS_STATUS_SUCCESS);

    // Initialize interrupts
    if (interruptsInit() != -1)
        printsys("Interrupts\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Interrupts\n", PRINTSYS_STATUS_FAIL);


    // TO BE TRANSFERED TO DEVICE MANAGER
    // Initialize the 8042 IBM PC PS/2 Controller
    if (ps2ControllerInit() != -1)
        printsys("8042 PS/2 Controller\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("8042 PS/2 Controller\n", PRINTSYS_STATUS_FAIL);

    // Initialize the ACPI mode
    if (ACPIinit(ACPI_CONTROL_FIND_FADT) != -1)
        printsys("ACPI\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("ACPI\n", PRINTSYS_STATUS_FAIL);

    // Initialise Keyboard
    if (keyboardInit(KEYBOARD_MODE_DISPLAY) != -1)
        printsys("Keyboard\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Keyboard\n", PRINTSYS_STATUS_FAIL);
}

void printsys(char* string, int status)
{
    if (status == PRINTSYS_STATUS_SUCCESS)
    {
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf("[ ");
        changeColor(green, black, CHANGE_COLOR_NEXT);
        printf("OK");
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf(" ]   ");
    }
    else if (status == PRINTSYS_STATUS_FAIL)
    {
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf("[ ");
        changeColor(red, black, CHANGE_COLOR_NEXT);
        printf("FAIL");
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf(" ] ");
    }

    changeColor(green, black, CHANGE_COLOR_NEXT);
    printf("%s", string);
}