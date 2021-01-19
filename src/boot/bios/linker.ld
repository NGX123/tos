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
        *(COMMON)
        *(.bss)

        /*Declare variable to hold the address where kernel ends(current address)*/
        kernel_end_addr = .;
    }
}