/*
    @author = ngx123
    @brief = main file from which function in others are started
*/


#include "headers/kernel.h"


void kernel_main(){
    struct FADT* FADTstruct;

    // Turns the cursor on and sets it size
    initScreen(CURSOR_ON);
    printsys("VGA Text Mode\n", PRINTSYS_STATUS_SUCCESS);


    // Initializes the platform specific stuff
    if (hardwarePlatformInit() != -1)
        printsys("Hardware Initialization\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Hardware Initialization\n", PRINTSYS_STATUS_SUCCESS);

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

    // Writes data to set the ACPI mode
    if ((FADTstruct = (struct FADT*)ACPIcontrol(ACPI_CONTROL_FIND_FADT)) != NULL){
        outb(FADTstruct->SMI_CommandPort, FADTstruct->AcpiEnable);  // Initialise the ACPI mode
        printsys("ACPI Detection\n", PRINTSYS_STATUS_SUCCESS);
    }
    else
        printsys("ACPI Detection\n", PRINTSYS_STATUS_FAIL);

    // Initialise Keyboard
    if (keyboardInit(KEYBOARD_MODE_DISPLAY) != -1)
        printsys("Keyboard\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Keyboard\n", PRINTSYS_STATUS_FAIL);
}
