/*
	@author = ngx123
	@brief = temporary kernel code for testing the kernel in long mode without interfering with the 32 bit kernel version(so it could also be ran)
*/


#include "include/kernel.h"


void kernel_main()
{
	// Call the bootloader function to initialize whatever it needs
	arch_bootloaderInterface(BOOTLOADER_FUNCTION_INIT);


	/* DEBUG CODE */
	initSerial();
	printSerial("\n\n---------------------------------------------------------------------------------------\n");

	printf("Hello");

	struct memInfo memory_map;
	int i;

	for (i = 0; ((memory_map = arch_getMemInfo(i, MEMMAP_TYPE_PROTOCOL)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
	{
		printf("addr = 0x%lx, length = 0x%lx, type = 0x%x\n", memory_map.start_address, memory_map.area_size, memory_map.area_type);
	}

	printf("MemMap by Interprter\n");

	for (i = 0; ((memory_map = arch_getMemInfo(i, MEMMAP_TYPE_INTERPRETER)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
	{
		printf("addr = 0x%lx, length = 0x%lx, type = 0x%x\n", memory_map.start_address, memory_map.area_size, memory_map.area_type);
	}


	while(1)
		;
}