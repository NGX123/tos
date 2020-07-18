// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:



#include "../drivers/drivers.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    write_string(red, black, "Faro");
    write_char(green, blue, 'A');
    init_serial();
    write_serial("faro");
    
    setFlat();
    // idt_init();
    
    
}

