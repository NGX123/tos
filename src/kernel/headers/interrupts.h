// File: interrupts.h
// Description: defines for platform independant interrupt code

#include "types.h"

#ifndef INTERRUPTS_LOCAL_H
#define INTERRUPTS_LOCAL_H

#define INTERRUPTS_AMOUNT 255
#define INTERRUPTS_HANDLERS_LIST_SIZE INTERRUPTS_AMOUNT+1

#define INTERRUPT_PRIORITY_SYSTEM 1
#define INTERRUPT_PRIORITY_KERNEL 2
#define INTERRUPT_PRIORITY_USER 3

#define INTERRUPT_STATUS_FREE 1
#define INTERRUPT_STATUS_REGISTERED 2


// Function type of an interrupt handler that is called when certain interrupt occures
typedef void (*interrupt_handler_t)(int);

// Function type of the kernels interrupt interpreter function
typedef int (*interrupt_interpreter_func_t)(int);

// A struct type containing info for the interrupt handler
struct interrupt_handler_struct {
    interrupt_handler_t function;
    int priority;
    int status;
};
// A type define for the interrupt handler structure
typedef struct interrupt_handler_struct interrupt_handler_struct_t;


// Give platform specifc code a pointer to systems main interrupt manager
/*
input:
    function - pointer to the kernel interrupt interpreter
return:
    on fail - -1
    on success - 0*/
extern int setInterruptInterpreterFunction(interrupt_interpreter_func_t function);

// Sends request to platform specific code to get numbers of the interrupts that should be reserved and stay unchanged
/*
input:
    numbers - pointer to a list that should be filled with interrupts that should be reserved
    list_size - maximum amount of interrupts
return:
    on fail - -1
    on success - amount of interrupt numbers in the list(can be 0)*/
extern int requestReservedInterrupts(int* numbers, int list_size);


// Initalizes the interrupt code ready to be used by the kernel
int interruptsInit();

// Change the function called by interrupt
int bindInterrupt(int interrupt_num, interrupt_handler_t handlerfunc, int priority);

// Is called by system interrupt handlers and is the main function to transfer interrupts to their desired location
static int interruptOccured(int interrupt_num);
#endif