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
    @brief = specific to hardware platform initializtion of interrupts and setting the kernels function that should be called to interrpret the interrupt
	@param function = pointer to the kernel interrupt interpreter
	@return = 0 on success, -1 on fail
*/
extern int arch_platformInterruptsInit(interrupt_interpreter_func_t function);

/*
	@breif = a request from architecture specific code to reserve some interrupt
	@param interrupt_num = the number of the interrupt that should be reserved(to not be used by kernel or software)
	@return = 0 on success, -1 on fail
*/
extern int interruptReserve(uint32_t interrupt_num);
#endif