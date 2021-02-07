/*
    @author = ngx123
    @brief = global header for kernel interrupt code
*/


#ifndef INTERRUPT_GLOBAL_H
#define INTERRUPT_GLOBAL_H


#define INTERRUPT_PRIORITY_UNINITIALIZED 0
#define INTERRUPT_PRIORITY_SYSTEM 1
#define INTERRUPT_PRIORITY_KERNEL 2
#define INTERRUPT_PRIORITY_USER 3


typedef void (*interrupt_handler_t)(void);          // Function type of an interrupt handler that is called when certain interrupt occures
typedef int (*interrupt_interpreter_func_t)(int);   // Function type of the kernels interrupt interpreter function


/*
    @brief = intializes the kernel side interrupt code
    @return = 0 on success, -1 on fail
*/
extern int interruptsInit(void);

/*
    @brief = binds a function to interrupt number, so it is called when an interrupt occures
    @param interrupt_num = number of the interrupt function should be binded to
    @param handlerfunc = pointer to the function that should be called
    @priority = the priority of the called
    @return = 0 on success, -1 on fail
*/
extern int bindInterrupt(int interrupt_num, interrupt_handler_t handlerfunc, int priority);

/*
    @brief = specific to hardware platform side initializtion of interrupts
*/
extern void platformInterruptsInit(void);

/*
    @brief = give platform specifc code a pointer to systems main interrupt interpreter function
    @param function = pointer to the kernel interrupt interpreter
    @return = 0 on success, -1 on fail
*/
extern int setInterruptInterpreterFunction(interrupt_interpreter_func_t function);

/*
    @brief = sends request to platform specific code to get numbers of the interrupts that should be reserved and stay unchanged
    @param numbers = pointer to a list that should be filled with interrupts that should be reserved
    @param list_size = maximum amount of interrupts to fill in the list with
    @return = amount of interrupt in the list on success, -1 on fail
*/
extern int requestReservedInterrupts(int* numbers, int list_size);
#endif