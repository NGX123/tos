// File: interrupts.c
// Description: is platform independant main part of interrupt handling code

#include "headers/kernel_interrupts.h"

// Interrupt handling functions list
static interrupt_handler_struct_t interrupt_list[INTERRUPTS_HANDLERS_LIST_SIZE];

// Is called by system interrupt handlers and is the main function to transfer interrupts to their desired location
static int interruptOccured(int interrupt_num){
    if (interrupt_list[interrupt_num].status == INTERRUPT_STATUS_FREE)
        return -1;

    interrupt_list[interrupt_num].function();

    return 0;
}

// Initalizes the interrupt code ready to be used by the kernel
int interruptsInit(){
    int reserved_interrupts[INTERRUPTS_AMOUNT];
    int reserved_interrupts_amount;

    // Initialize the interrupts(hardware specific)
    platformInterruptsInit();

    // Indicate the end of the list
    interrupt_list[INTERRUPTS_HANDLERS_LIST_SIZE-1].function = NULL;

    // Setup the platform specific functions with the right function pointer(kernel interrupt handler) to call when interrupt occures
    if (setInterruptInterpreterFunction(&interruptOccured) == -1)
        return -1;

    // Reserve the platform specific special interrupts
    if ((reserved_interrupts_amount = requestReservedInterrupts(reserved_interrupts, INTERRUPTS_AMOUNT)) == -1)
        return -1;
    else{
        for (int i = 0; i < reserved_interrupts_amount; i++)
            if (reserved_interrupts[i] < INTERRUPTS_AMOUNT)
                interrupt_list[reserved_interrupts[i]].priority = INTERRUPT_PRIORITY_SYSTEM;
    }

    return 0;
}

// Change the function called by interrupt
int bindInterrupt(int interrupt_num, interrupt_handler_t handlerfunc, int priority){
    // Fail if the interrupt number is to large or callers priority is too low
    if (interrupt_num >= INTERRUPTS_AMOUNT || (priority > interrupt_list[interrupt_num].priority && interrupt_list[interrupt_num].status != INTERRUPT_STATUS_FREE))
        return -1;

    interrupt_list[interrupt_num].function = handlerfunc;

    return 0;
}