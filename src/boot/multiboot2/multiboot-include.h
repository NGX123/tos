// @author = ngx123
// @brief = includes defines for multiboot2 bootstrap code


#include <stdint.h>
#include <types.h>
#include <stdarg.h>
#include <kernel/init.h>
#include "string.h
#include <kernel/multiboot2.h>
#include <kernel/multiboot.h>


#define TOGGLE_BIT_ON   1
#define TOGGLE_BIT_OFF  0


/*
    @brief = changes the chosen bit in supplied variable to chosen value(ON(1) or OFF(0))
    @param var = pointer to variable where the changes will be made
    @param bitmask = the bit which has to be changed
    @param bit_status = to which status the bit should be changed
*/
static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status);

/*
    @brief = constantly returns filled memInfo structs with new entries from mem map
    @param count = variable which contains the number of the entrys that should be read, sets error bit when entries list reaches end, variable should be increased by user
    @return = filled memInfo struct
*/
static struct memInfo getMultibootMemInfo(int count);

/*
    @brief = solves all of the platform specfic stuff for the kernel
    @param protocol = type of boot protocol used
    @param var_num = amount of arguments to be passed
    @param ... = the platform specific arguments
*/
extern void setBootInfo(int var_num, ...);