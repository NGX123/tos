/*
    @author = ngx123
    @brief = includes defines for multiboot2 bootstrap code
*/

#include <stdint.h>
#include <stdarg.h>
#include <arch/init.h>
#include <kernel/multiboot2.h>
#include <kernel_types.h>
#include "string.h"


#define TOGGLE_BIT_ON   1
#define TOGGLE_BIT_OFF  0

#define AVAILABLE_FLAG_MEMSIZE      0x1
#define AVAILABLE_FLAG_MEMMAP       0x2
#define AVAILABLE_FLAG_FRAMEBUFFER  0x4

/*
    @brief = changes the chosen bit in supplied variable to chosen value(ON(1) or OFF(0))
    @param var = pointer to variable where the changes will be made
    @param bitmask = the bit which has to be changed
    @param bit_status = to which status the bit should be changed
*/
static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status);

/*
    @breif = recieves data from a multiboot2 bootloader and interprets it
    @param magic = magic number to check if the OS was booted by a multiboot2 compilent bootloader
    @param infoStruct_addr = pointer to multiboot information structure
*/
extern void interpretMultiboot2(void);