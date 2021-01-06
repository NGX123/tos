/*
    @author = ngx123
    @brief = local defines for kernel interrupt handling code
*/


#ifndef INTERRUPTS_LOCAL_H
#define INTERRUPTS_LOCAL_H


#include "types.h"
#include "interrupts.h"


#define INTERRUPTS_AMOUNT 255
#define INTERRUPTS_HANDLERS_LIST_SIZE INTERRUPTS_AMOUNT+1

#define INTERRUPT_STATUS_OFF 0
#define INTERRUPT_STATUS_ON 1


// A struct type containing info for the interrupt handler
struct interrupt_handler_struct {
    interrupt_handler_t function;
    int priority;
    int status;
};
// A type define for the interrupt handler structure
typedef struct interrupt_handler_struct interrupt_handler_struct_t;


/*
    @brief = function called by platform specific interrupt handler that interprets the interrupt
    @param interrupt_num = the number of interrupt used to look the information from the list about it
    @return = 0 on success, -1 on fail
*/
static int interruptOccured(int interrupt_num);
#endif