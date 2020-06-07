#include "vga_print.h"


void kernel_main(){
    make_attributes(green, red);
    write_string(color, "Faro");
}