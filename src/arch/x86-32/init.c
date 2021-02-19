/*
    @author = ngx123
    @brief = code for setting up GDT
*/


#include "include/init.h"


static struct GDTentry gdt32[3]; // Array is used to hold all of the GDT entries as they go one after another
static struct GDTdescriptor gdt32_ptr;


void makeMemFlat()
{
    // Null Selector
    gdt32[0].limit_low      = 0;
    gdt32[0].base_low       = 0;
    gdt32[0].base_middle    = 0;
    gdt32[0].access         = 0;
    gdt32[0].granularity    = 0;
    gdt32[0].base_high      = 0;

    // Code Selector
    gdt32[1].limit_low      = 0xffff;
    gdt32[1].base_low       = 0;
    gdt32[1].base_middle    = 0;
    gdt32[1].access         = 0x9A; // 10011010
    gdt32[1].granularity    = 0xCF; // 11001111
    gdt32[1].base_high      = 0;

    // Data Selector
    gdt32[2].limit_low      = 0xffff;
    gdt32[2].base_low       = 0;
    gdt32[2].base_middle    = 0;
    gdt32[2].access         = 0x92; // 10010010
    gdt32[2].granularity    = 0xCF; // 11001111
    gdt32[2].base_high      = 0;

    // GDT Descriptor
    gdt32_ptr.limit = sizeof(gdt32) - 1;
    gdt32_ptr.base = &gdt32;

    gdtLoadAsm(&gdt32_ptr);
}
