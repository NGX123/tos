/*
    @author = ngx123
    @brief = includes code needed to get data and setup everything using multiboot2
*/


#include "multiboot-include.h"


extern uint64_t multiboot_magic_var;									/* External variables linking to the ones stored in the .bss section that hold values supplied by grub */
extern uint64_t multiboot_tags_address_var;

static unsigned long multiboot2_magic_copy;								/* Spaces to store multiboot magic and tags start pointer locally(so they are not stored in some random place in .bss section and can be possibly lost) */
static address_tt multiboot2_tags_address_copy;

static struct multiboot_tag* multiboot2_tags_start_address_converted;	// The address that skipes directly to the start of tags structures without the size and other fields

static unsigned long multiboot2_all_tags_size;							// Tags structures toatal size variable, for use in mmap(to mark the space where the tags exist as reserved in the page frame allocator)

static uint16_t avilability_flags;


int  arch_bootloaderInterface(uint32_t function)
{
	if (function == BOOTLOADER_FUNCTION_INIT)
		return interpretMultiboot2();

	return BOOTLOADER_RETURN_WRONG_FUNCTION;
}

static int interpretMultiboot2(void)
{
	struct multiboot_tag *multiboot2_tag_current;

	if (multiboot_magic_var != MULTIBOOT2_BOOTLOADER_MAGIC)
		return BOOTLOADER_RETURN_WRONG_PROTOCOL;
	if (multiboot_tags_address_var & 7)																	// Make a check if the address tags address is not aligned by 8
		return BOOTLOADER_RETURN_ADDRESS_PROBLEM;

	multiboot2_magic_copy = multiboot_magic_var;														/* COPY THE MAGIC number and TAGS ptr to local variables for safety */
	multiboot2_tags_address_copy = multiboot_tags_address_var;

	multiboot2_all_tags_size = *(uint32_t*)multiboot_tags_address_var;									// Tags address points to a structure where the first field is size(with 32 bit size)
	multiboot2_tags_start_address_converted = (struct multiboot_tag*)(multiboot_tags_address_var + 8); 	// As tags address points to a structure, to get to pointer to tags, skip the first two fields

	for (multiboot2_tag_current = multiboot2_tags_start_address_converted; multiboot2_tag_current->type != MULTIBOOT_TAG_TYPE_END; multiboot2_tag_current = (struct multiboot_tag*)((multiboot_uint8_t*)multiboot2_tag_current + ((multiboot2_tag_current->size + 7) & ~7)))	// Loop through all tags(using proccess described in multiboot2 documentation) and do the needed things for chosen ones(with cases)
	{
		switch (multiboot2_tag_current->type)
		{
			case MULTIBOOT_TAG_TYPE_MMAP:
				toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_MEMMAP, TOGGLE_BIT_ON);
				break;
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_FRAMEBUFFER, TOGGLE_BIT_ON);
				break;
		}
	}

	return BOOTLOADER_RETURN_SUCCESS;
}

struct memInfo arch_getMemInfo(int count, uint8_t mmap_type)
{
    struct memInfo returnStruct = {0};
    struct multiboot_tag *tag_current;;
    multiboot_memory_map_t *mmap;
	int interpreter_mmap_entry_amount = 0;
	int interpreter_mmap_entry_fill_counter = 0;

    if (!(avilability_flags & AVAILABLE_FLAG_MEMMAP))									// Return error if the bootloader hasn't given the memory map
    {
        toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
        return returnStruct;
    }


	if (multiboot2_tags_address_copy != 0 && multiboot2_all_tags_size != 0) 			// Increment the counter amount of times same as amount of entries bootloader wants to add to memory_map
		interpreter_mmap_entry_amount++;

	struct memInfo interpreter_mmap[interpreter_mmap_entry_amount];

	if (multiboot2_tags_address_copy != 0 && multiboot2_all_tags_size != 0)				/* Fill in the first entry which is the multiboot structure address and size */
	{
		interpreter_mmap[interpreter_mmap_entry_fill_counter].start_address = multiboot2_tags_address_copy;
		interpreter_mmap[interpreter_mmap_entry_fill_counter].area_size     = multiboot2_all_tags_size;
		interpreter_mmap[interpreter_mmap_entry_fill_counter].area_type	  = MEMMAP_AREA_TYPE_RESERVED;
		interpreter_mmap_entry_fill_counter++;
	}

	if (mmap_type == MEMMAP_TYPE_PROTOCOL)
	{
		for (tag_current = multiboot2_tags_start_address_converted; tag_current->type != MULTIBOOT_TAG_TYPE_END; tag_current = (struct multiboot_tag *)((multiboot_uint8_t *)tag_current + ((tag_current->size + 7) & ~7)))	// Parse the memory map and put it in the kernels format(proccess described in multiboot 2 spec)
			if (tag_current->type == MULTIBOOT_TAG_TYPE_MMAP)
			{
				mmap = ((struct multiboot_tag_mmap *) tag_current)->entries;
				mmap = (multiboot_memory_map_t *) ((uint64_t)mmap + (((struct multiboot_tag_mmap *)tag_current)->entry_size * count));

				if ((multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag_current + tag_current->size)
				{
					returnStruct.start_address = mmap->addr;
					returnStruct.area_size = mmap->len;
					returnStruct.area_type = mmap->type;
				}
				else
				{
					toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
					return returnStruct;
				}
			}
	}
	else if (mmap_type == MEMMAP_TYPE_INTERPRETER)
	{
		if (count < interpreter_mmap_entry_amount)
		{
			returnStruct.start_address 	= interpreter_mmap[count].start_address;
			returnStruct.area_size 		= interpreter_mmap[count].area_size;
			returnStruct.area_type		= interpreter_mmap[count].area_type;
		}
		else
		{
			toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
			return returnStruct;
		}
	}
	else
	{
		toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
		return returnStruct;
	}

    return returnStruct;
}

static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status)
{
    if ((bit_status == TOGGLE_BIT_ON && (*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && !(*var & bitmask)))
        ;
    else if ((bit_status == TOGGLE_BIT_ON && !(*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && (*var & bitmask)))
        *var ^= bitmask;
}




/* TOMPORARY IMPLEMENTATION FOR DEBUGGING */
int putchar(int chara)
{
    writeSerial(chara);
    return 0;
}