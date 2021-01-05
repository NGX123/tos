// File: kernel.h
// Description: kernel code header

#ifndef KERNEL_DEFINES_H
#define KERNEL_DEFINES_H

/// Includes
#include <stdint.h>
#include "string.h"
#include "stdio.h"
#include "libk.h"

#include "drivers/vga.h"
#include "drivers/x86.h"
#include "drivers/acpi.h"
#include "drivers/kbd.h"
#include "drivers/ps2_8042.h"
#include "interrupts.h"
#include "platform.h"

/// Declarations
// Core function of the kernel that is called by bootloader
void kernel_main();
#endif