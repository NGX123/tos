/*
    @author = ngx123
    @brief = includes code needed to get data and setup everything using stivale2(https://github.com/stivale/stivale/blob/master/STIVALE2.md)
*/


#include "stivale-include.h"


static uint8_t stack[16384];								// 16 KB reserved for the stack
static uint16_t avilability_flags;							// Has flags for stuff that bootloader has supplied
static struct stivale2_tag* stivale2_tags_struct_ptr;

struct stivale2_header_tag_framebuffer framebuffer_request = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next       = 0,									// 0 means that it is the end of the linked list of tags
    },
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0,
};

struct stivale2_header __attribute__((section(".stivale2hdr"), used)) header2 = {	// The stivale2 header structure is defined with GCC attributes: section - indicates that the variable should be placed in non-default section chosen by user; used - indicates that the compiler should make the variable even if it is not used anywhere in the code
    .entry_point = 0,										// Entry point should be 0 so the one specified in elf header will be used
    .stack       = (uintptr_t)&stack + sizeof(stack),		// Address of the space that is reserved for the stack
    .flags       = 0,										// All bits are undefined and should be zero
    .tags        = (uint64_t)&framebuffer_request			// Pointer to the first member in a linked list of tags
};


void _start(struct stivale2_struct* info)	/* The function has struct pointer as an argument because stivale protocol printf the pointer to it's info struct into rdi which in "System V AMD64 ABI Calling Convention" is used to store the first argument that is an integer/pointer(not the stack as in i386 ABI) */
{
	stivale2_tags_struct_ptr = (struct stivale2_tag *)info->tags;
	struct stivale2_tag* tag_current;

	for (tag_current = stivale2_tags_struct_ptr; tag_current != NULL; tag_current = (struct stivale2_tag *)tag_current->next)
	{
		switch (tag_current->identifier)
		{
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_MEMMAP, TOGGLE_BIT_ON);
                break;
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_FRAMEBUFFER, TOGGLE_BIT_ON);
                break;
            case STIVALE2_STRUCT_TAG_RSDP_ID:
				toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_ACPI, TOGGLE_BIT_ON);
                break;
            default:
                break;
        }
	}

	kernel_setup();
}

struct memInfo arch_getMemInfo(size_t count, uint8_t mmap_type)
{
    struct memInfo returnStruct = {0};
	struct stivale2_tag *tag_current;
	struct stivale2_struct_tag_memmap* mmap;
	struct stivale2_mmap_entry mmap_entry;

	if (!(avilability_flags & AVAILABLE_FLAG_MEMMAP))									// Return error if the bootloader hasn't given the memory map
    {
        toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
        return returnStruct;
    }

	if (mmap_type == MEMMAP_TYPE_PROTOCOL)
	{
		for (tag_current = stivale2_tags_struct_ptr; tag_current != NULL; tag_current = (struct stivale2_tag *)tag_current->next)	// Parse the memory map and put it in the kernels format(proccess described in multiboot 2 spec)
			if (tag_current->identifier == STIVALE2_STRUCT_TAG_MEMMAP_ID)
			{
				mmap = (struct stivale2_struct_tag_memmap*)tag_current;

				if (count < mmap->entries)
				{
					mmap_entry = mmap->memmap[count];

					returnStruct.start_address = mmap_entry.base;
					returnStruct.area_size = mmap_entry.length;
					if (mmap_entry.type == STIVALE2_MMAP_USABLE)
						returnStruct.area_type = MEMMAP_AREA_TYPE_USABLE;
					else if (mmap_entry.type == STIVALE2_MMAP_RESERVED)
						returnStruct.area_type = MEMMAP_AREA_TYPE_RESERVED;
					else
						returnStruct.area_type = MEMMAP_AREA_TYPE_OTHER;
				}
				else
				{
					toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
					return returnStruct;
				}
			}
	}
	else
	{
		toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
		return returnStruct;
	}

	return returnStruct;
}

int arch_bootloaderInterface(uint32_t function)
{
	if (function == BOOTLOADER_FUNCTION_INIT){
		return BOOTLOADER_RETURN_SUCCESS;
	}

	return BOOTLOADER_RETURN_WRONG_FUNCTION;
}

static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status)
{
    if ((bit_status == TOGGLE_BIT_ON && (*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && !(*var & bitmask)))
        ;
    else if ((bit_status == TOGGLE_BIT_ON && !(*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && (*var & bitmask)))
        *var ^= bitmask;
}