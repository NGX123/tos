// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:



#include "kernel.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    prints(green, black, "Initialised: VGA, ");
    
    init_serial();
    write_serial("Serial test run");
    prints(green, black, "Initialised: Serial, ");

    setFlat();
    // Initialisation notice in assembly
    
    idt_init();
    // Initialization notice in assembly 

    
    
    
    
    
}

