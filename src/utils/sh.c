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

static void format_input(char input[]){
    int i = 0, j = 0, k = 0, len = 0, words = 0;
    int* word_starts;
    int word_length;

    /*
    + Strip the string free of tabs and enter symbols, 
    make a int list with start of every variable
    add a variable with amount of elements at the end of list
    remove all spaces and reallocate it for the new size
    then for every string just make a temp vairbale and then put it into large malloced one for passing to the exec(as a separate variable)
    */

    // Removes consecutive zeros
    len = strlen(input);
    for(i = 0; i < len;)
    {
        if(input[i] == ' ' && input[i] == input[i+1])
        {
            for(j = i; j < len; j++)
                input[j] = input[j+1];
            --len;
        }
        else
            i++;
    }
    i = 0; j = 0; len = 0; 
    
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

    // Remove spaces at the end of the string 
    len = strlen(input);
    for (i = len; input[i] == 0 || input[i] == ' '; i--)
        if (input[i] == ' ')
            input[i] = 0;
    i = 0; j = 0; len = 0; 

    // Remove spaces at the start of the string
    len = strlen(input);
    for (i = 0; input[i] == ' '; i++)
        for (j = i; j < len; j++)
                input[j] = input[j+1];
            --len;  
    i = 0; j = 0; len = 0; 

    // Counts words in the string
    if (strlen(input) > 1){
        words = 1;
        for (i = 0; input[i] != '\0'; i++)
            if (input[i] == ' ' && input[i+1] != '\0' && input[i+1] != ' ')
                ++words;
    }
    i = 0; j = 0; len = 0; 

    // Output the string after cleaning
    printf("\nString - %s\nAmount of words = %d\nAll symbols - ", input, words);
    for (i = 0; input[i] != 0; i++){
        if (input[i] == '\n')
            printf("\\n\\");
        else if (input[i] == '\t')
            printf("\\t\\");
        else if (input[i] == ' ')
            printf("\\b\\");
        else 
            printf("%c", input[i]);
    }
    printf("\n");
    i = 0; j = 0; len = 0; 

    // Get the starts of the words
    word_starts = malloc(sizeof(int) * 2);
    word_starts[j++] = 0;
    for (i = 1; input[i] != 0; i++){
        if (input[i] != ' ' && input[i-1] == ' '){
            word_starts[j++] = i;
            word_starts = realloc(word_starts, sizeof(int) * (j + 1));
        }
    }
    i = 0; len = 0; //j = 0;
    
    // Print the starts of words
    printf("\n");
    for (i = 0; i < j; i++)
        printf("Word starts - %d\n", word_starts[i]);
    i = 0; len = 0; //j = 0;

    // Put words into malloced list
    char **exec_input = malloc(sizeof(char*) * words);
    len = strlen(input);
    for (i = 0, k = 0; i < j; i++){
        if (k != (j - 1))
            word_length = word_starts[k+1] - 1 - word_starts[k++];
        else 
            word_length = len - word_starts[k++];

        exec_input[i] = malloc(sizeof(char) * word_length);
        printf("\n");
        printf("Word length - %d", word_length);
    }
        


    // Free memory
    free(word_starts);
    free(input);
}

int main(){
    char* input_buffer = NULL;
    size_t size = 0;

    while(1){
        getline(&input_buffer, &size, stdin);
        format_input(input_buffer);
    }
    

    return 0;
}