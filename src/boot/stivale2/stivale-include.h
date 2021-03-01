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


/*
	@breif = the function that is called by the stivale2 bootloader(specified in the linkerscript)
	@param info = the address of the stivale info struct(stored in the rdi which is the storage of the first pointer/integer function argument according to System V AMD64 ABI Calling Convention)
*/
extern void _start(struct stivale2_struct* info);
