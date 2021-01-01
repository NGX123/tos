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

    // Writes data to set the ACPI mode
    if ((FADTstruct = (struct FADT*)ACPIcontrol(ACPI_CONTROL_FIND_FADT)) != NULL){
        outb(FADTstruct->SMI_CommandPort, FADTstruct->AcpiEnable);  // Initialise the ACPI mode
        printsys("ACPI Detection\n", PRINTSYS_STATUS_SUCCESS);
    }
    else
        printsys("ACPI Detection\n", PRINTSYS_STATUS_FAIL);

    // Sets the flat memory mode and initializes GDT
    setFlat();

    // Initialize interrupts
    if (interruptsInit() != -1)
        printsys("Interrupts\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Interrupts\n", PRINTSYS_STATUS_FAIL);

    // Bind keyboard interrupt
    if (bindInterrupt(1, &keyboard_handler, INTERRUPT_PRIORITY_KERNEL) != -1)
        printsys("Keyboard interrupts\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Keyboard interrupts\n", PRINTSYS_STATUS_FAIL);

    // Writes to serial port to configure it
    initSerial();
    writeSerial("Serial test run");
    printsys("Serial Port\n", PRINTSYS_STATUS_SUCCESS);

    // Initialise Keyboard
    if (keyboardInit(KEYBOARD_MODE_DISPLAY) != -1)
        printsys("Keyboard\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Keyboard\n", PRINTSYS_STATUS_FAIL);
}
