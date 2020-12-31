// File: kernel.c
// Description: this is the main file which starts all other files

#include "headers/kernel.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    struct FADT* FADTstruct;

    // Turns the cursor on and sets it size
    initScreen(CURSOR_ON);
    printsys("Initialised: Screen\n");

    // Writes data to set the ACPI mode
    if ((FADTstruct = (struct FADT*)ACPIcontrol(ACPI_CONTROL_FIND_FADT)) != NULL){
        outb(FADTstruct->SMI_CommandPort, FADTstruct->AcpiEnable);  // Initialise the ACPI mode
        printsys("Initialised: ACPI\n");
    }

    // Initialize interrupts
    interruptsInit();

    // Sets the flat memory mode and initializes GDT
    setFlat();

    // Writes to serial port to configure it
    initSerial();
    writeSerial("Serial test run");
    printsys("Initialised: Serial\n");

    // Initialise Keyboard
    if (keyboardInit(KEYBOARD_MODE_DISPLAY) != -1)
        printsys("Initialised: Keyboard\n");
}
