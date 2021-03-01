/*
    @author = ngx123
    @brief = includes code needed to get data and setup everything using stivale2(https://github.com/stivale/stivale/blob/master/STIVALE2.md)
*/


#include "stivale-include.h"

#include <stdio.h>
#include "../../drivers/include/serial.h"


static uint8_t stack[16384];								// 16 KB reserved for the stack

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
	/* DEBUG CODE */
	initSerial();
	printSerial("\n\n---------------------------------------------------------------------------------------\n");


	struct stivale2_tag* tags_ptr = (struct stivale2_tag *)info->tags;
	struct stivale2_tag* tag_current;

	for (tag_current = tags_ptr; tag_current != NULL; tag_current = (struct stivale2_tag *)tag_current->next)
	{
		switch (tag_current->identifier) {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
                struct stivale2_struct_tag_memmap *m = (struct stivale2_struct_tag_memmap *)tag_current;
                printf("----- Memmap tag: ----- \n");
                printf("Entries: %d\n", m->entries);
                for (size_t i = 0; i < m->entries; i++) {
                    struct stivale2_mmap_entry me = m->memmap[i];
                    printf("Base: %x, Size: %x, Type: %x\n", me.base, me.length, me.type);
                }
                break;
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
                struct stivale2_struct_tag_framebuffer *f = (struct stivale2_struct_tag_framebuffer *)tag_current;
                printf("----- Framebuffer tag: -----\n");
                printf("Address: %x\n", f->framebuffer_addr);
                printf("Width:   %d\n", f->framebuffer_width);
                printf("Height:  %d\n", f->framebuffer_height);
                printf("Pitch:   %d\n", f->framebuffer_pitch);
                printf("BPP:     %d\n", f->framebuffer_bpp);
                printf("Memory model:    %d\n", f->memory_model);
                printf("Red mask size:   %d\n", f->red_mask_size);
                printf("Red mask size:   %d\n", f->red_mask_shift);
                printf("Green mask size: %d\n", f->green_mask_size);
                printf("Green mask size: %d\n", f->green_mask_shift);
                printf("Blue mask size:  %d\n", f->blue_mask_size);
                printf("Blue mask size:  %d\n", f->blue_mask_shift);
                break;
            }
            case STIVALE2_STRUCT_TAG_RSDP_ID: {
                break;
            }
            default:
                printf("----- Unidentifier tag %x -----\n", tag_current->identifier);
        }
	}

	kernel_setup();
}

int arch_bootloaderInterface(uint32_t function)
{
	if (function == BOOTLOADER_FUNCTION_INIT){
		printf("Hello");
		return 0;
	}

	return -1;
}




/* TEMPORARY IMPLEMENTATION FOR DEBUGGING */
int putchar(int chara)
{
    writeSerial(chara);
    return 0;
}