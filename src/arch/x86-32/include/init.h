/*
    @author = ngx123
    @brief = local header with defines for the GDT initialization code
*/


#ifndef GDT_DEFINES_H
#define GDT_DEFINES_H


#include <stdint.h>


struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    uint16_t limit;
    void* base;
} __attribute__((packed));


/*
    @brief = Loads the GDT in to the CPU
    @param void* = Address of the GDT struct to load
*/
extern void load_gdt(void*);

/*
    @brief = Sets up the GDT and fills it with data and then loads setting up the flat memory model
*/
static void setFlat();
#endif