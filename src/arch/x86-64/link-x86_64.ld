/*Function that will be marked as the entry point(and will be called by bootloader*/
ENTRY(_start)

/*Change section arrangement in memory*/
SECTIONS
{
    /*Change current address to change starting from where sections will be placed(current address specified by .), 0x00100000 - 1Mb(2^20)*/
    . = 0x00100000;

    /*Select .text sections and align it to 4KB(4096), write the rules for linking it(in {})*/
    .text ALIGN(4K) :
    {
		_kernel_start = .; /* Variable is located in the text section because it is the first section mapped in this linkerscript(which means it is start of kernel) and variable should be inside a section or otherwise it could cause undefined behaviour, same with end variable but it is just in the last mapped section */

		*(.multiboot)	/*Connect .multiboot sections from all files into one, it is under .text to have .text(code) section permissions. All the follwing rules do the same thing but just with different sections and permission*/
        *(.text)
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