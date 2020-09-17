// File: kernel.c
// Description: this is the main file which starts all other files



#include <stdint.h>
#include "string.h"
#include "stdio.h"
#include "libk.h"

#include "drivers/vga.h"
#include "drivers/x86.h"
#include "drivers/acpi.h"
#include "drivers/kbd.h"

extern void initSerial();
extern void writeSerial(char* a);
extern void  setFlat();
extern void  idt_init();

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

    // Sets the flat memory mode and initializes GDT
    setFlat();
    
    // Initializes IDT
    idt_init();
        
    // Writes to serial port to configure it
    initSerial();
    writeSerial("Serial test run");
    printsys("Initialised: Serial\n");

    // Initialise Keyboard
    if (keyboardInit(KEYBOARD_MODE_DISPLAY) != -1)
        printsys("Initialised: Keyboard\n");
}

