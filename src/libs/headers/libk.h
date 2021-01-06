/*
    @author = ngx123
    @brief = header for library with special function only available to kernel
*/


#ifndef LIBK_H
#define LIBK_H


#define PRINTSYS_STATUS_SUCCESS 0
#define PRINTSYS_STATUS_FAIL 1


/*
    @brief = prints a system initialization message
    @param string = the string to print
    @param status = the status of the print - FAIL/SUCCESS
*/
extern void printsys(char* string, int status);

/*
    @brief = prints a GDT initialization message
*/
extern void display_gdt();

/*
    @brief = prints an IDT initialization message
*/
extern void display_idt();
#endif