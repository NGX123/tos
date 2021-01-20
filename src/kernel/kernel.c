/*
    @author = ngx123
    @brief = main file from which function in others are started
*/


#include "include/kernel.h"


struct bootHeader bootHeader_struct;


void kernel_init(uint16_t protocol, int var_num, ...)
{
    va_list valist;
    va_start(valist, var_num);

    void* headerPtr;

    if (protocol == PROTOCOL_MULTIBOOT){
        if (va_arg(valist, uint32_t) != MULTIBOOT_BOOTLOADER_MAGIC)             // Check if the multiboot magic number is present
            return;

        headerPtr = va_arg(valist, void*);
        if ((((multiboot_info_t*)headerPtr)->flags & (1<<6)) == 0)              // Check if memory map flag is on
            return;

        bootHeader_struct.struct_reserved_start_addr = headerPtr;
        bootHeader_struct.struct_reserved_end_addr = headerPtr + sizeof(multiboot_info_t);
        bootHeader_struct.memory_map_start_addr = (void*)((multiboot_info_t*)headerPtr)->mmap_addr;
        bootHeader_struct.memory_map_end_addr = bootHeader_struct.memory_map_start_addr + ((multiboot_info_t*)headerPtr)->mmap_length;
    }

    bootHeader_struct.protocol = protocol;

    va_end(valist);
}

void kernel_main()
{
    // Turns the cursor on and sets it size
    initScreen(CURSOR_ON);
    printsys("VGA Text Mode\n", PRINTSYS_STATUS_SUCCESS);


    // Initializes the platform specific stuff
    if (hardwarePlatformInit() != -1)
        printsys("Hardware Initialization\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Hardware Initialization\n", PRINTSYS_STATUS_SUCCESS);

    // Initialize interrupts
    if (interruptsInit() != -1)
        printsys("Interrupts\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Interrupts\n", PRINTSYS_STATUS_FAIL);


    // TO BE TRANSFERED TO DEVICE MANAGER
    // Initialize the 8042 IBM PC PS/2 Controller
    if (ps2ControllerInit() != -1)
        printsys("8042 PS/2 Controller\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("8042 PS/2 Controller\n", PRINTSYS_STATUS_FAIL);

    // Initialize the ACPI mode
    if (ACPIinit(ACPI_CONTROL_FIND_FADT) != -1)
        printsys("ACPI\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("ACPI\n", PRINTSYS_STATUS_FAIL);

    // Initialise Keyboard
    if (keyboardInit(KEYBOARD_MODE_DISPLAY) != -1)
        printsys("Keyboard\n", PRINTSYS_STATUS_SUCCESS);
    else
        printsys("Keyboard\n", PRINTSYS_STATUS_FAIL);
}
