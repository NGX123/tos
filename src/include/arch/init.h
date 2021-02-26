/*
    @author = ngx123
    @brief = global defines for hardware platform initialization code
*/


#ifndef INIT_H
#define INIT_H


#include <kernel_types.h>


#define MEMINFO_FLAG_ERROR       	0x1

#define MEMMAP_AREA_TYPE_USABLE     0x1
#define MEMMAP_AREA_TYPE_RESERVED   0x2
#define MEMMAP_AREA_TYPE_SPECIAL    0x3 // Area that was reserved by kernel
#define MEMMAP_AREA_TYPE_OTHER      0x4 // Bootloader should be asked about this memory using bootloaderInterface()

#define MEMMAP_TYPE_PROTOCOL		0x0	// Make getmeminfo return the memory map supplied by bootloader protocol
#define MEMMAP_TYPE_INTERPRETER		0x1	// Make getmeminfo return the memory map additional fields created by the boot protocol interpreter(e.g. multiboot2.c)

#define BOOTLOADER_FUNCTION_INIT 	0x1

#define BOOTLOADER_RETURN_SUCCESS			0
#define BOOTLOADER_RETURN_WRONG_PROTOCOL 	0x1
#define BOOTLOADER_RETURN_ADDRESS_PROBLEM	0x2


struct memInfo
{
    uint8_t flags;

	address_tt start_address;
	address_tt area_size;
    uint32_t area_type;
};

/*
    @brief = fills memInfo structure based on "count". Sets error flag if "count" is bigger then amount of entries in the memory map. Example use is - calling memInfo with count 0, reading data from memInfo, incrementing count by one, doing previous steps until error flag is set
    @param count = specifies the entry in the memory map
    @return = filled in memory structure
*/
extern struct memInfo arch_getMemInfo(int count, uint8_t mmap_type);

/*
    @breif = reads needed information from the bootloader based on command and responds with it
*/
extern int arch_bootloaderInterface(uint32_t function);
#endif