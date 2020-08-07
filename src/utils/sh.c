#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int buffer_size = 1, var_size = 1;
char** buffer;

void format_input(char input[]){
    // Error checking
    if ((buffer = malloc(sizeof(char*) * buffer_size)) == NULL){
        printf("Failed to initialize the buffer - Allocation Error\n");
        exit(1);
    }
    int i, j = 0, k, i_current, i_next;

    for (i = 0; input[i] != 0; i++)
        if (input[i] == '\n')
            input[i] = 0;

    for (i = 0; input[i] != '\0'; i++){
        if (input[i] != ' ')
            ++var_size;
        else {
            --var_size;
            
            if ((buffer[j] = malloc(sizeof(char) * (var_size+1))) == NULL){
                printf("Failed to allocate space for the varible\n");
                exit(1);
            };

            for (k = 0; k < var_size; k++)
                buffer[j][k] = input[k];

            buffer[j][k] = 0;
            var_size = 0;
            ++j;
            ++buffer_size;

            // Error checking 
            if ((buffer = realloc(buffer, sizeof(char*) * buffer_size)) == NULL){
                printf("Failed to reallocate buffer\n");
                exit(1);
            }
        }   
    }

    
    
    
    
    // for (i = 0; input[i] != 0; i++){
    //     if (input[i] == ' ' || input[i] == '\t')
    //         for (j = i, i_current = i, i_next = i + 1; input[j - 1] != 0; j++){
    //             input[i_current] = input[i_next];

    //             i_current = i_next;
    //             ++i_next;
    //         }
    // }
        


}


int main(){
    char* input_buffer = NULL;
    size_t size = 0;

    while(1){
        getline(&input_buffer, &size, stdin);
        format_input(input_buffer);
        
        for (int i = 0; i < buffer_size; i++)
            printf("%s", buffer[i]);

        // if (strcmp(input_buffer, "exit") == 0)
        //     exit(1);
        // else if (strcmp(input_buffer, "cd") == 0)
        //     chdir("");

        
    }
    

    return 0;
}

