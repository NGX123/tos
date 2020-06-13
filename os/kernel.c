// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:



#include "vga_print.h"
#include "serial.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    
    write_string(red, black, "Faro");
    init_serial();
    write_serial("faro");
}

