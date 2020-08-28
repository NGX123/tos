// File: libk.c
// Description: includes special function that will be available only to the kernel



#include "stdio.h"

void printsys(char* string){
    printf("[ INIT ] %s", string);
}

// Info prints ///
void display_gdt(){
    printsys("Initialized: GDT\n");
}

void display_idt(){
    printsys("Initialised: IDT\n");
}