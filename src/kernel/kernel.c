// File: kernel.c
// Description: this is the main file which starts all other files



#include "kernel.h"
#include "x86.h"
#include "string.h"
#include "stdio.h"
#include "acpi.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    struct FADT* FADTstruct;

    // Turns the cursor on and sets it size
    enable_cursor(0, 15);
    printk("Initialised: Screen\n");

    // Sets the flat memory mode and initializes GDT
    setFlat();
    
    // Initializes IDT
    idt_init();

    // Writes data to set the ACPI mode
    if ((FADTstruct = (struct FADT*)ACPIcontrol(1)) != NULL){
        outb(FADTstruct->SMI_CommandPort, FADTstruct->AcpiEnable);  // Initialise the ACPI mode
        printk("Initialised: ACPI\n");
    }
        
    // Writes to serial port to configure it
    init_serial();
    write_serial("Serial test run");
    printk("Initialised: Serial\n");

    
}

