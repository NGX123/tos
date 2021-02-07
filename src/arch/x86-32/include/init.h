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
#include <platform.h>
#include "string.h"
#include "stdio.h"


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
// static void initPaging();
#endif