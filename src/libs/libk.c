// File: libk.c
// Description: includes special function that will be available only to the kernel

#include "stdio.h"
#include "drivers/vga.h"
#include "libk.h"

void printsys(char* string, int status){
    // Success Message
    if (status == PRINTSYS_STATUS_SUCCESS){
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf("[ ");
        changeColor(green, black, CHANGE_COLOR_NEXT);
        printf("OK");
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf(" ]   ");
    }
    // Fail Message
    else if (status == PRINTSYS_STATUS_FAIL){
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf("[ ");
        changeColor(red, black, CHANGE_COLOR_NEXT);
        printf("FAIL");
        changeColor(white, black, CHANGE_COLOR_NEXT);
        printf(" ] ");
    }

    // Message
    changeColor(green, black, CHANGE_COLOR_NEXT);
    printf("%s", string);
}

// Info prints ///
void display_gdt(){
    printsys("Initialized: GDT\n", PRINTSYS_STATUS_SUCCESS);
}

void display_idt(){
    printsys("Initialised: IDT\n", PRINTSYS_STATUS_SUCCESS);
}