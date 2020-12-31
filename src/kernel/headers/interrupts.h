// File: interrupts.h
// Description: defines for platform independant interrupt code

#ifndef INTERRUPTS_LOCAL_H
#define INTERRUPTS_LOCAL_H

#include "types.h"
#include "kernel_interrupts.h"

#define INTERRUPTS_AMOUNT 255
#define INTERRUPTS_HANDLERS_LIST_SIZE INTERRUPTS_AMOUNT+1

#define INTERRUPT_STATUS_FREE 1
#define INTERRUPT_STATUS_REGISTERED 2


// A struct type containing info for the interrupt handler
struct interrupt_handler_struct {
    interrupt_handler_t function;
    int priority;
    int status;
};
// A type define for the interrupt handler structure
typedef struct interrupt_handler_struct interrupt_handler_struct_t;


// Is called by system interrupt handlers and is the main function to transfer interrupts to their desired location
static int interruptOccured(int interrupt_num);
#endif