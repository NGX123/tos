/*
	@author = ngx123
	@brief = temporary kernel code for testing the kernel in long mode without interfering with the 32 bit kernel version(so it could also be ran)
*/


#include "include/kernel.h"


void kernel_main()
{
	// Call the bootloader function to initialize whatever it needs
	arch_bootloaderInterface(BOOTLOADER_FUNCTION_INIT);
}