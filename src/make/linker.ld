/* Bootloader will look at this image and start execution at entry point symbol*/
ENTRY(_start)

/* Tell where idfferent sections of object files will be put in the final kernel image*/
SECTIONS
{
    /* Begin putting sections at 1MiB, a place for kernels to be loaded by bootloaders */
    . = 1M;

    /*Make .text and .multiboot sections follow .text permissions, align and fix size*/
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Make .rodata section follow .rodata permissions, align and fix size */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Read write data */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Read-write data and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
