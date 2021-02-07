/*
    @author = ngx123
    @brief = header file for the kernel
*/


#ifndef KERNEL_DEFINES_H
#define KERNEL_DEFINES_H


#define PRINTSYS_STATUS_SUCCESS 0
#define PRINTSYS_STATUS_FAIL 1


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <drivers/vga.h>
#include <arch/io_x86.h>
#include <drivers/acpi.h>
#include <drivers/kbd.h>
#include <drivers/ps2_8042.h>
#include <kernel/interrupts.h>
#include <platform.h>
#include <kernel/multiboot.h>
#include <kernel/init.h>


/*
    @brief = main function of the kernel that calls all other functions
*/
extern void kernel_main();

/*
    @brief = prints a system initialization message
    @param string = the string to print
    @param status = the status of the print - FAIL/SUCCESS
*/
extern void printsys(char* string, int status);
#endif