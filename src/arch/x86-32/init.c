/*
    @author = ngx123
    @brief = code for setting up GDT
*/


#include "include/init.h"


static struct GDTentry gdt32[3]; // Array is used to hold all of the GDT entries as they go one after another
static struct GDTdescriptor gdt32_ptr;


void makeMemFlat()
{
    // Null Descriptor
    gdt32[0].limit_low = 0x0000;
    gdt32[0].base_low = 0x0000;
    gdt32[0].base_middle = 0x00;
    gdt32[0].access = 0x00;
    gdt32[0].granularity = 0x00;
    gdt32[0].base_high = 0x00;

    // Code Descriptor
    gdt32[1].limit_low = 0xffff;
    gdt32[1].base_low = 0x0000;
    gdt32[1].base_middle = 0x00;
    gdt32[1].access = 0x9A;
    gdt32[1].granularity = 0xCF;
    gdt32[1].base_high = 0x00;

    // Data Descriptor
    gdt32[2].limit_low = 0xffff;
    gdt32[2].base_low = 0x0000;
    gdt32[2].base_middle = 0x00;
    gdt32[2].access = 0x92;
    gdt32[2].granularity = 0xCF;
    gdt32[2].base_high = 0x00;

    // GDT Descriptor
    gdt32_ptr.limit = sizeof(gdt32) - 1;
    gdt32_ptr.base = &gdt32;

    gdtLoadAsm(&gdt32_ptr);
}
