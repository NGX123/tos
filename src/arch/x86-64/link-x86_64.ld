/*Place the code right at the start of .text*/
ENTRY(_start)

/*Arrange sections*/
SECTIONS
{
    /*Change current address to change starting from where sections will be placed(current address specified by .), 0x00100000 - 1Mb(2^20)*/
    . = 0x00100000;

    /*Put .multiboot & .text sections from all files into one .text section and align it to 4Kb*/
    .text ALIGN(4K) :
    {
		_kernel_start = .;			/* Variable to count the kernel size, it is placed in text section because it the first section that is plced in memory in this linkerscript and if the varible is outside section, a linkerscript creates a new one for it which can have undefined behaviour*/
        *(.multiboot)
        *(.text)
    }

    /*Put .rodata sections from all files into one .rodata section and align it to 4Kb*/
    .rodata ALIGN(4K) :
    {
        *(.rodata)
    }

    /*Put .data sections from all files into one .data section and align it to 4Kb*/
    .data ALIGN(4K) :
    {
        *(.data)
    }

    /*Put .bss and ....... sections from all files into one .bss section and align it to 4Kb*/
    .bss ALIGN(4K) :
    {
        *(.bss*)
		_kernel_end = .;
    }
}