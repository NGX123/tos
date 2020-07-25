// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:



#include "kernel.h"
#include "x86.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    prints(green, black, "Initialised: VGA, ");
    
    init_serial();
    write_serial("Serial test run");
    prints(green, black, "Initialised: Serial, ");

    setFlat();
    
    idt_init();

    //enable_cursor(start, end);
    //prints(green, black, "Initialised: Cursor, ");
}

