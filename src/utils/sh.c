// File: sh.c
// Description: system shell
// Functions: getline, malloc, realloc
// Problems: double spaces occur after cleaning in some combinations of tabs and spaces



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int buffer_size = 1, var_size = 1;
static char** buffer;
char **exec_input;

static void format_input(char input[]){
    char delimeter[2] = " ";
    char* token;

    printf("\n\n\n");
    token = strtok(input, delimeter);
    while( token != NULL ) {
      printf( " %s\n", token );
    
      token = strtok(NULL, delimeter);
    }
    
    
    // // Removes '\t' and '\n'
    // len = strlen(input);
    // for (i = 0; i < len;){
    //     if (input[i] == '\n')
    //         input[i++] = 0;
    //     else if (input[i] == '\t'){
    //         for (j = i; j < len; j++)
    //             input[j] = input[j+1];
    //         --len;  
    //     }
    //     else 
    //         ++i;
    // }     
    // i = 0; j = 0; len = 0;

    
}

int main(){
    int i, j;
    char* input_buffer = NULL;
    size_t size = 0;

    while(1){
        getline(&input_buffer, &size, stdin);
        format_input(input_buffer);
    }
    
    //free(input_buffer);
    return 0;
}






    /*
    + Strip the string free of tabs and enter symbols, 
    make a int list with start of every variable
    add a variable with amount of elements at the end of list
    remove all spaces and reallocate it for the new size
    then for every string just make a temp vairbale and then put it into large malloced one for passing to the exec(as a separate variable)
    */