// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:



#include "kernel.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    
    

    prints(green, black, "Initialised: Display, ");
    
    init_serial();
    write_serial("Serial test run");
    prints(green, black, "Initialised: Serial, ");

    setFlat();
    // Initialisation notice in assembly
    
    outb(0x20, 0x11);
    outb(0xA0, 0x11); 
    outb(0x21, 0x20); 
    outb(0xA1, 40); 
    outb(0x21, 0x04); 
    outb(0xA1, 0x02); 
    outb(0x21, 0x01);     
    outb(0xA1, 0x01); 
    outb(0x21, 0x0); 
    outb(0xA1, 0x0);
    idt_init();
    // Initialization notice in assembly 

    
    
    
    
    
}

