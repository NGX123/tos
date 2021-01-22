/*
    @author = ngx123
    @brief = main file from which function in others are started
*/


#include "include/kernel.h"


void kernel_main()
{
    // Turns the cursor on and sets it size
    initScreen(CURSOR_ON);
    printsys("VGA Text Mode\n", PRINTSYS_STATUS_SUCCESS);

    // Memory detection
    struct kernInfo* kernInfo_struct = getBootInfo();
    if (kernInfo_struct->protocol != PROTOCOL_NONE){
        printsys("RAM Detected ", PRINTSYS_STATUS_SUCCESS);
        printf("- Lower: %d(0x%p) KiB, Upper: %d(0x%p) KiB\n", ((multiboot_info_t*)kernInfo_struct->boot_protocol_struct_ptr)->mem_lower, ((multiboot_info_t*)kernInfo_struct->boot_protocol_struct_ptr)->mem_lower, kernInfo_struct->high_ram_amount, kernInfo_struct->high_ram_amount);
    }
    else
        printsys("RAM Detection", PRINTSYS_STATUS_FAIL);

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
