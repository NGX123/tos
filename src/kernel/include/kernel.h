/*
    @author = ngx123
    @brief = header file for the kernel
*/


#ifndef KERNEL_DEFINES_H
#define KERNEL_DEFINES_H


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <libk.h>

#include <drivers/vga.h>
#include <drivers/x86.h>
#include <drivers/acpi.h>
#include <drivers/kbd.h>
#include <drivers/ps2_8042.h>
#include <interrupts.h>
#include <platform.h>


/*
    @brief = main function of the kernel that calls all other functions
*/
extern void kernel_main();
#endif