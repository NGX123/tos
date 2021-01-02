// File: kernel.c
// Description: this is the main file which starts all other files

#include "headers/kernel.h"
extern void keyboard_handler();

// Core function of the kernel that is called by bootloader
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
