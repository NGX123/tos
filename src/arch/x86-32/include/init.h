/*
    @author = ngx123
    @brief = local header with defines for the GDT initialization code
*/


#ifndef INIT_DEFINES_H
#define INIT_DEFINES_H


#include <stdint.h>
#include <types.h>
#include <stdarg.h>
#include <kernel/init.h>
#include "string.h"

#define TOGGLE_BIT_ON   1
#define TOGGLE_BIT_OFF  0


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
extern void gdtLoadAsm(void*);

/*
    @brief = Sets up the GDT and fills it with data and then loads setting up the flat memory model
*/
static void makeMemFlat();

/*
    @brief = Sets up memory paging
*/
static void initPaging();

/*
    @brief = changes the chosen bit in supplied variable to chosen value(ON(1) or OFF(0))
    @param var = pointer to variable where the changes will be made
    @param bitmask = the bit which has to be changed
    @param bit_status = to which status the bit should be changed
*/
static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status);

/*
    @brief = constantly returns filled memInfo structs with new entries from mem map
    @param count = variable which contains the number of the entrys that should be read(it is automatically increased by the function and is set to -1 when func comes to an end)
    @return = filled memInfo struct
*/
static struct memInfo getMultibootMemInfo(int* count);
#endif