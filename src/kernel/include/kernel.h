/*
    @author = ngx123
    @brief = header file for the kernel
*/


#ifndef KERNEL_DEFINES_H
#define KERNEL_DEFINES_H


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/libk.h>
#include <stdarg.h>

#include <drivers/vga.h>
#include <drivers/x86.h>
#include <drivers/acpi.h>
#include <drivers/kbd.h>
#include <drivers/ps2_8042.h>
#include <kernel/interrupts.h>
#include <platform.h>
#include <kernel/multiboot.h>


#define PROTOCOL_MULTIBOOT  0x01


struct bootHeader
{
    uint16_t protocol;
    void* struct_reserved_start_addr;
    void* struct_reserved_end_addr;
    void* memory_map_start_addr;
    void* memory_map_end_addr;
};


/*
    @brief = solves all of the platform specfic stuff for the kernel
    @param protocol = type of boot protocol used
    @param var_num = amount of arguments to be passed
    @param ... = the platform specific arguments
*/
extern void kernel_init(uint16_t protocol, int var_num, ...);

/*
    @brief = main function of the kernel that calls all other functions
*/
extern void kernel_main();
#endif