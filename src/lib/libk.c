/*
    @author = ngx123
    @brief = library with special function only available to kernel
*/


#include <stdio.h>
#include <drivers/vga.h>
#include <libk.h>


void printsys(char* string, int status)
{
    if (status == PRINTSYS_STATUS_SUCCESS)
    {
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf("[ ");
        changeColor(green, black, CHANGE_COLOR_NEXT);
        printf("OK");
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf(" ]   ");
    }
    else if (status == PRINTSYS_STATUS_FAIL)
    {
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf("[ ");
        changeColor(red, black, CHANGE_COLOR_NEXT);
        printf("FAIL");
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf(" ] ");
    }

    changeColor(green, black, CHANGE_COLOR_NEXT);
    printf("%s", string);
}

void display_gdt()
{
    printsys("GDT\n", PRINTSYS_STATUS_SUCCESS);
}

void display_idt()
{
    printsys("IDT\n", PRINTSYS_STATUS_SUCCESS);
}