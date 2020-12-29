// File: interrupts.h
// Description: includes global defines for the interrupts handling part of the kernel

#define INTERRUPT_PRIORITY_SYSTEM 1
#define INTERRUPT_PRIORITY_KERNEL 2
#define INTERRUPT_PRIORITY_USER 3

typedef void (*interrupt_handler_t)(int);
typedef int (*interrupt_interpreter_func_t)(int);


// Initalizes the interrupt code ready to be used by the kernel
int interruptsInit();

// Change the function called by interrupt
int bindInterrupt(int interrupt_num, interrupt_handler_t handlerfunc, int priority);


// Give platform specifc code a pointer to systems main interrupt manager
/*
input:
    function - pointer to the kernel interrupt interpreter
return:
    on fail - -1
    on success - 0*/
int setInterruptInterpreterFunction(interrupt_interpreter_func_t function);

// Sends request to platform specific code to get numbers of the interrupts that should be reserved and stay unchanged
/*
input:
    numbers - pointer to a list that should be filled with interrupts that should be reserved
    list_size - maximum amount of interrupts
return:
    on fail - -1
    on success - amount of interrupt numbers in the list(can be 0)*/
int requestReservedInterrupts(int* numbers, int list_size);