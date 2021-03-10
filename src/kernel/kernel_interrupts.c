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
    if (arch_platformInterruptsInit(&interruptOccured) == -1)
		return -1;

    interrupt_list[INTERRUPTS_HANDLERS_LIST_SIZE-1].function = NULL;		// Set the status of all interrupts to unknown

    return 0;
}

int interruptReserve(uint32_t interrupt_num)
{
	if (interrupt_num < INTERRUPTS_AMOUNT)
		interrupt_list[interrupt_num].priority = INTERRUPT_PRIORITY_SYSTEM;
	else
		return -1;

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