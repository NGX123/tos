// File: kernel.c
// Description: this is the main file which starts all other files



#include "driver.h"
#include "x86.h"
#include "string.h"
#include "stdio.h"
#include "acpi.h"
#include "libk.h"

extern void  setFlat();
extern void  idt_init();
extern int keyboardInit(struct FADT* FADTstruct, uint8_t mode);

// Core function of the kernel that is called by bootloader
void kernel_main(){
    struct FADT* FADTstruct;

    // Turns the cursor on and sets it size
    initScreen();
    printsys("Initialised: Screen\n");

    // Sets the flat memory mode and initializes GDT
    setFlat();
    
    // Initializes IDT
    idt_init();

    // Writes data to set the ACPI mode
    if ((FADTstruct = (struct FADT*)ACPIcontrol(1)) != NULL){
        outb(FADTstruct->SMI_CommandPort, FADTstruct->AcpiEnable);  // Initialise the ACPI mode
        printsys("Initialised: ACPI\n");
    }
        
    // Writes to serial port to configure it
    initSerial();
    writeSerial("Serial test run");
    printsys("Initialised: Serial\n");

    // Initialise Keyboard
    if (keyboardInit(FADTstruct, 1) != -1)
        printsys("Initialised: Keyboard\n");   
}

