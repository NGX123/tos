/*
    @author = ngx123
    @brief = includes defines for stivale2 bootstrap code
*/


#include <stdint.h>
#include <stdarg.h>
#include <arch/init.h>
#include <kernel_types.h>

#include "string.h"
#include "stivale2.h"


#define AVAILABLE_FLAG_MEMMAP       0x1
#define AVAILABLE_FLAG_FRAMEBUFFER  0x2
#define AVAILABLE_FLAG_ACPI			0x4

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
	@breif = the function that is called by the stivale2 bootloader(specified in the linkerscript)
	@param info = the address of the stivale info struct(stored in the rdi which is the storage of the first pointer/integer function argument according to System V AMD64 ABI Calling Convention)
*/
extern void _start(struct stivale2_struct* info);
