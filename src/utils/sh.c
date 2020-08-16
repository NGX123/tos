// File: sh.c
// Description: system shell
// Functions: getline, malloc, realloc, free, strtok, strlen, strcpy, printf
// Problems:



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char **exec_input;

static void format_input(char* input){
    char delimeter[2] = " ";
    char* token;
    int i, j, len, buff_size = 1;

    // Removes '\t' and '\n'
    len = strlen(input);
    for (i = 0; i < len;){
        if (input[i] == '\n')
            input[i++] = 0;
        else if (input[i] == '\t'){
            for (j = i; j < len; j++)
                input[j] = input[j+1];
            --len;  
        }
        else 
            ++i;
    }     
    i = 0; j = 0; len = 0;

    // Put tokens into a list
    exec_input = malloc(sizeof(char*) * buff_size);
    token = strtok(input, delimeter);
    for(i = 0; token != NULL; i++) {
        exec_input[i] = malloc(sizeof(char) * (strlen(token)+1));
        strcpy(exec_input[i], token);
        exec_input = realloc(exec_input, sizeof(char*) * ++buff_size);
        token = strtok(NULL, delimeter);
    }
    exec_input[i] = NULL;
    i = 0;

    printf("\n\n\n");
    for (i = 0; exec_input[i] != NULL; i++)
        printf("%s\n", exec_input[i]);
    
    




}

int main(){
    int i, j;
    char* input_buffer = NULL;
    size_t size = 0;

    while(1){
        getline(&input_buffer, &size, stdin);
        format_input(input_buffer);
        
        // Exit
        if (strcmp(exec_input[0], "exit") == 0){
            for (i = 0; exec_input[i] != NULL; i++)
                free(exec_input[i]);
            free(exec_input);
            free(input_buffer);
            exit(0);
        }    

        // Free buffers
        for (i = 0; exec_input[i] != NULL; i++)
                free(exec_input[i]);
        free(exec_input);
    }
    
    free(input_buffer);
    return 0;
}

    // Removes consecutive zeros
    // len = strlen(input);
    // for(i = 0; i < len;)
    // {
    //     if(input[i] == ' ' && input[i] == input[i+1])
    //     {
    //         for(j = i; j < len; j++)
    //             input[j] = input[j+1];
    //         --len;
    //     }
    //     else
    //         i++;
    // }
    // i = 0; j = 0; len = 0; 

    /*
    + Strip the string free of tabs and enter symbols, 
    make a int list with start of every variable
    add a variable with amount of elements at the end of list
    remove all spaces and reallocate it for the new size
    then for every string just make a temp vairbale and then put it into large malloced one for passing to the exec(as a separate variable)
    */