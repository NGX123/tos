/*
    @author = ngx123
    @brief = includes code needed to get data and setup everything using stivale2(https://github.com/stivale/stivale/blob/master/STIVALE2.md)
*/


#include "stivale2.h"
#include <kernel_types.h>

static uint8_t stack[16384];							// 16 KB reserved for the stack


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
    .stack       = &stack + sizeof(stack),					// Address of the space that is reserved for the stack
    .flags       = 0,										// All bits are undefined and should be zero
    .tags        = (uint64_t)&framebuffer_request			// Pointer to the first member in a linked list of tags
};


void _start(struct stivale2_struct* info)	/* The function has struct pointer as an argument because stivale protocol puts the pointer to it's info struct into rdi which in "System V AMD64 ABI Calling Convention" is used to store the first argument that is an integer/pointer(not the stack as in i386 ABI) */
{
	struct stivale2_tag* tags_ptr = (struct stivale2_tag *)info->tags;
	struct stivale2_tag* tag_current;

	for (tag_current = tags_ptr; tag_current != NULL; tag_current = tag_current->next)
	{
		switch (tag_current->identifier) {
            case STIVALE2_STRUCT_TAG_CMDLINE_ID: {
                struct stivale2_struct_tag_cmdline *c = (struct stivale2_struct_tag_cmdline *)tag_current;
                e9_puts("Commandline tag:");
                e9_printf("\tCmdline: %s", (char*)(c->cmdline));
                break;
            }
            case STIVALE2_STRUCT_TAG_MEMMAP_ID: {
                struct stivale2_struct_tag_memmap *m = (struct stivale2_struct_tag_memmap *)tag_current;
                e9_puts("Memmap tag:");
                e9_printf("\tEntries: %d", m->entries);
                for (size_t i = 0; i < m->entries; i++) {
                    struct stivale2_mmap_entry me = m->memmap[i];
                    e9_printf("\t\t[%x+%x] %x", me.base, me.length, me.type);
                }
                break;
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID: {
                struct stivale2_struct_tag_framebuffer *f = (struct stivale2_struct_tag_framebuffer *)tag_current;
                e9_puts("Framebuffer tag:");
                e9_printf("\tAddress: %x", f->framebuffer_addr);
                e9_printf("\tWidth:   %d", f->framebuffer_width);
                e9_printf("\tHeight:  %d", f->framebuffer_height);
                e9_printf("\tPitch:   %d", f->framebuffer_pitch);
                e9_printf("\tBPP:     %d", f->framebuffer_bpp);
                e9_printf("\tMemory model:    %d", f->memory_model);
                e9_printf("\tRed mask size:   %d", f->red_mask_size);
                e9_printf("\tRed mask size:   %d", f->red_mask_shift);
                e9_printf("\tGreen mask size: %d", f->green_mask_size);
                e9_printf("\tGreen mask size: %d", f->green_mask_shift);
                e9_printf("\tBlue mask size:  %d", f->blue_mask_size);
                e9_printf("\tBlue mask size:  %d", f->blue_mask_shift);
                break;
            }
            case STIVALE2_STRUCT_TAG_FB_MTRR_ID: {
                e9_puts("Framebuffer WC MTRR tag:");
                e9_puts("\tFramebuffer WC MTRR enabled");
                break;
            }
            case STIVALE2_STRUCT_TAG_MODULES_ID: {
                struct stivale2_struct_tag_modules *m = (struct stivale2_struct_tag_modules *)tag_current;
                e9_puts("Modules tag:");
                e9_printf("\tCount: %d", m->module_count);
                for (size_t i = 0; i < m->module_count; i++) {
                    struct stivale2_module me = m->modules[i];
                    e9_printf("\t\t[%x+%x] %s", me.begin, me.end, me.string);
                }
                break;
            }
            case STIVALE2_STRUCT_TAG_RSDP_ID: {
                struct stivale2_struct_tag_rsdp *r = (struct stivale2_struct_tag_rsdp *)tag_current;
                e9_puts("RSDP tag:");
                e9_printf("\tRSDP: %x", r->rsdp);
                break;
            }
            case STIVALE2_STRUCT_TAG_EPOCH_ID: {
                struct stivale2_struct_tag_epoch *e = (struct stivale2_struct_tag_epoch *)tag_current;
                e9_puts("Epoch tag:");
                e9_printf("\tEpoch: %x", e->epoch);
                break;
            }
            case STIVALE2_STRUCT_TAG_FIRMWARE_ID: {
                struct stivale2_struct_tag_firmware *f = (struct stivale2_struct_tag_firmware *)tag_current;
                e9_puts("Firmware tag:");
                e9_printf("\tFlags: %x", f->flags);
                break;
            }
            default:
                e9_printf("BUG: Unidentifier tag %x", tag_current->identifier);
        }
	}
}