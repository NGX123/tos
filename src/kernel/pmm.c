/*
    @author = ngx123
    @breif = physicaly memory manager(page frame allocator)
*/


#include "include/pmm.h"
#include <stdio.h>


size_t RAMsize, freeRAMsize, reservedRAMsize, allocatedRAMsize;
size_t RAMstart;

uint8_t* bitmap_ptr;
address_tt bitmap_addr;
size_t bitmap_size;

address_tt kernel_addr;
size_t kernel_size;


int initPMM()
{
	if ((RAMsize = getRAMsize()) == 0)
		return -1;

	if (initBitmap() == -1)
		return -1;

	if (interpretMemoryMap() == -1)
		return -1;

	printf("Start - 0x%lx, Size - 0x%lx, Free - 0x%lx, Reserved - 0x%lx, Allocated - 0x%lx\n", RAMstart, RAMsize, freeRAMsize, reservedRAMsize, allocatedRAMsize);

	return 0;
}

void* palloc(size_t frame_count)
{
	size_t first_frame_index = 0;
	size_t free_frames_count = 0;
	size_t free_frames_start_index = 0;
	void* free_frames_start_addr = NULL;

	for (size_t current_frame_index = first_frame_index; current_frame_index < bitmap_size; current_frame_index++)
	{
		if (free_frames_count == frame_count && free_frames_start_addr != NULL)
		{
			for (size_t i = 0; i < frame_count; i++)
				bitmap_ptr[free_frames_start_index+i] = FRAME_TYPE_ALLOCATED;

			return free_frames_start_addr;
		}

		if (bitmap_ptr[current_frame_index] == FRAME_TYPE_FREE)
		{
			free_frames_count++;
			if (free_frames_start_addr == NULL)
			{
				free_frames_start_addr = (void*)((uintptr_t)current_frame_index * FRAME_SIZE);
				free_frames_start_index = current_frame_index;
			}

		}

		else if (bitmap_ptr[current_frame_index] != FRAME_TYPE_FREE)
		{
			free_frames_start_addr = NULL;
			free_frames_count = 0;
		}

	}

	return NULL;
}

int pfree(address_tt frame_address, size_t frame_count)
{
	size_t first_frame_index = (size_t)(frame_address / FRAME_SIZE);
	if (first_frame_index >= bitmap_size)
		return -1;

	for (size_t current_frame_index = first_frame_index; frame_count > 0 && current_frame_index < bitmap_size; frame_count--, current_frame_index++)
	{
		if (bitmap_ptr[current_frame_index] == FRAME_TYPE_ALLOCATED)
			bitmap_ptr[current_frame_index] = FRAME_TYPE_FREE;
		else
			return -1;

		freeRAMsize += FRAME_SIZE;
		allocatedRAMsize -= FRAME_SIZE;
	}

	return 0;
}

int reserveRAM(address_tt frame_address, size_t frame_count)
{
	size_t first_frame_index = (size_t)(frame_address / FRAME_SIZE);
	if (first_frame_index >= bitmap_size)
		return -1;

	for (size_t current_frame_index = first_frame_index; frame_count > 0 && current_frame_index < bitmap_size; frame_count--, current_frame_index++)
	{
		if (bitmap_ptr[current_frame_index] == FRAME_TYPE_FREE || bitmap_ptr[current_frame_index] == FRAME_TYPE_RESERVED)
			bitmap_ptr[current_frame_index] = FRAME_TYPE_RESERVED;
		else
			return -1;

		freeRAMsize -= FRAME_SIZE;
		reservedRAMsize += FRAME_SIZE;
	}

	return 0;
}

int unreserveRAM(address_tt frame_address, size_t frame_count)
{
	size_t first_frame_index = (size_t)(frame_address / FRAME_SIZE);
	if (first_frame_index >= bitmap_size)
		return -1;

	for (size_t current_frame_index = first_frame_index; frame_count > 0 && current_frame_index < bitmap_size; frame_count--, current_frame_index++)
	{
		if (bitmap_ptr[current_frame_index] == FRAME_TYPE_RESERVED)
			bitmap_ptr[current_frame_index] = FRAME_TYPE_FREE;
		else
			return -1;

		freeRAMsize += FRAME_SIZE;
		reservedRAMsize -= FRAME_SIZE;
	}

	return 0;
}

static int interpretMemoryMap()
{
    struct memInfo mmap_entry;
    for (size_t i = 0; ((mmap_entry = arch_getMemInfo(i, MEMMAP_TYPE_PROTOCOL)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
    {
		if (mmap_entry.area_type == MEMMAP_AREA_TYPE_USABLE)
		{
			if (unreserveRAM(mmap_entry.start_address, (mmap_entry.area_size / FRAME_SIZE)) == -1)
				return -1;
		}
	}

	if (reserveRAM(kernel_addr, (kernel_addr / FRAME_SIZE)) == -1)
		return -1;

	return 0;
}

static int initBitmap()
{
	int kernel_area_presence_status = 0;
	struct memInfo mmap_entry;

	address_tt largest_mmap_area_addr;
	size_t largest_mmap_area_size = 0;

	for (size_t i = 0; ((mmap_entry = arch_getMemInfo(i, MEMMAP_TYPE_PROTOCOL)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
    {
		if (i == 0)												/* Set the start of RAM */
			RAMstart = mmap_entry.start_address;

		if (mmap_entry.area_type == MEMMAP_AREA_TYPE_KERNEL) 	/* Check if the kernel area is present in the memory map */
			kernel_area_presence_status = 1;

		if (mmap_entry.area_type == MEMMAP_AREA_TYPE_USABLE && mmap_entry.area_size > largest_mmap_area_size)	/* Finding the largest free memory area */
		{
			largest_mmap_area_addr = mmap_entry.start_address;
			largest_mmap_area_size = mmap_entry.area_size;
		}

		if (mmap_entry.area_type == MEMMAP_AREA_TYPE_KERNEL)													/* Get the location and size of the kernel */
		{
			if (kernel_size != 0)
				return -1;

			kernel_addr = mmap_entry.start_address;
			kernel_size = mmap_entry.area_size;
		}
	}

	if (kernel_area_presence_status == 0)						/* Fail if kernel area is not specified in the memory map */
		return -1;

	bitmap_addr = largest_mmap_area_addr;
	bitmap_size = RAMsize / FRAME_SIZE;
	uint8_t bitmap_kernel_collision = 0;						// 0 if Kernel and bitmap do not collide in memory(some or all addresses are the same), 1 if they collide
	address_tt largest_mmap_area_end = largest_mmap_area_addr + largest_mmap_area_size - 1;
	address_tt kernel_end = kernel_addr + kernel_size - 1;
	address_tt bitmap_end = bitmap_addr + bitmap_size - 1;


	if (bitmap_end >= largest_mmap_area_end)					// Check if there is enough free memory in the largest free memory section
		return -1;

	if (bitmap_addr > kernel_addr)								/* Check if the kernels location and bitmaps possible location(start of largest free area) collide in memory */
	{
		if (bitmap_addr < kernel_end)							// If bitmap start address is after the kernel start address, but the bitmap end address is below the kernel start address - there is collision
			bitmap_kernel_collision = 1;
	}
	else if (bitmap_end > kernel_addr)							// If bitmap start address is below the kernel start address, but the bitmap end address is after the kernel start address - there is collision
		bitmap_kernel_collision = 1;

	if (bitmap_kernel_collision == 1)
	{
		bitmap_addr = kernel_end + 1;							// If there is collision change the start of the bitmap to byte aftere end of kernel
		bitmap_end = bitmap_addr + bitmap_size - 1;
		if (bitmap_end >= largest_mmap_area_end)				// Check if there is enough free memory left in the section to fit the bitmap if it's address is changed to end of kernel
			return -1;
	}

	bitmap_ptr = (uint8_t*)bitmap_addr;

	for (size_t i = 0; i < bitmap_size; i++)	// Set all the memory to reserved in the memory map
	{
		bitmap_ptr[i] = MEMMAP_AREA_TYPE_RESERVED;
		reservedRAMsize += FRAME_SIZE;
	}

	return 0;
}

static size_t getRAMsize()
{
    size_t ram_size = 0;
    struct memInfo mmap_entry;

    for (size_t i = 0; ((mmap_entry = arch_getMemInfo(i, MEMMAP_TYPE_PROTOCOL)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
    {
        ram_size += mmap_entry.area_size;
		printf("addr = 0x%lx, length = 0x%lx, type = 0x%x\n", mmap_entry.start_address, mmap_entry.area_size, mmap_entry.area_type);
	}

    return ram_size;
}