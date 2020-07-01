// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:

#include "stdint.h"
#include "x86.h"
#include "vga-driver.h"
#include "serial-driver.h"


// Core function of the kernel that is called by bootloader
void kernel_main(){
    
    write_string(red, black, "Faro");
    write_char(green, blue, 'A');
    init_serial();
    write_serial("faro");
}

