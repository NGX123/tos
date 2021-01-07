/*
    @author = ngx123
    @brief = main platform independant interrupt handling part of kernel
*/


#include "include/kernel_interrupts.h"


// Interrupt handling functions list
static interrupt_handler_struct_t interrupt_list[INTERRUPTS_HANDLERS_LIST_SIZE];


static int interruptOccured(int interrupt_num)
{
    if (interrupt_list[interrupt_num].status == INTERRUPT_STATUS_OFF)
        return -1;

    interrupt_list[interrupt_num].function();

    return 0;
}

int interruptsInit()
{
    int reserved_interrupts[INTERRUPTS_AMOUNT];
    int reserved_interrupts_amount;

    platformInterruptsInit();

    interrupt_list[INTERRUPTS_HANDLERS_LIST_SIZE-1].function = NULL;

    if (setInterruptInterpreterFunction(&interruptOccured) == -1)
        return -1;

    if ((reserved_interrupts_amount = requestReservedInterrupts(reserved_interrupts, INTERRUPTS_AMOUNT)) == -1)
        return -1;
    else
    {
        for (int i = 0; i < reserved_interrupts_amount; i++)
            if (reserved_interrupts[i] < INTERRUPTS_AMOUNT)
                interrupt_list[reserved_interrupts[i]].priority = INTERRUPT_PRIORITY_SYSTEM;
            else
                return -1;
    }

    return 0;
}

int bindInterrupt(int interrupt_num, interrupt_handler_t handlerfunc, int priority)
{
    if (interrupt_num >= INTERRUPTS_AMOUNT || (priority > interrupt_list[interrupt_num].priority && interrupt_list[interrupt_num].priority != INTERRUPT_PRIORITY_UNINITIALIZED))
        return -1;

    interrupt_list[interrupt_num].function = handlerfunc;
    interrupt_list[interrupt_num].status = INTERRUPT_STATUS_ON;
    interrupt_list[interrupt_num].priority = priority;

    return 0;
}