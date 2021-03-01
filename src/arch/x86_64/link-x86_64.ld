/*
	@author = ngx123
	@brief = linkerscript for x86_64 that is directly loaded by bootloader into the long mode(if there is a need for linkerscript for kernel that is 64bit and higher half, but is loaded inot protected mode - create a new linkerscript)
*/

/*Function that will be marked as the entry point(and will be called by bootloader*/
ENTRY(_start)

/*Change section arrangement in memory*/
SECTIONS
{
    /*Change current address to change starting from where sections will be placed(current address specified by .)*/
    /* Lower half kernel address(just to test with GRUB if needed */
	/* . = 0x00100000; */
	. = 0xffffffff80100000;

	/* A hacky way to have the kernel be compatible with GRUB, bootboot and tomatboot/stivale because only the last one depends on the how the section with it's header is called and other bootloaders dont't care in what section their header is so the stivale's section name is used*/
    .stivale2hdr ALIGN(4K) :
    {
		/* All the possible names of sections that could have the bootloader header */
        *(.stivale2hdr)
		*(.multiboot)
		*(.bootloaderhdr)
    }

    /*Select .text sections and align it to 4KB(4096), write the rules for linking it(in {})*/
    .text ALIGN(4K) :
    {
		_kernel_start = .; /* Variable is located in the text section because it is the first section mapped in this linkerscript(which means it is start of kernel) and variable should be inside a section or otherwise it could cause undefined behaviour, same with end variable but it is just in the last mapped section */
        *(.text)			/*Connect .text sections from all files into one and have the code/data in them be loacted at the start of the text section*/
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata)
    }

    .data ALIGN(4K) :
    {
        *(.data)
    }

    .bss ALIGN(4K) :
    {
        *(.bss*)
		_kernel_end = .;
    }
}