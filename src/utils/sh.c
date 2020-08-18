// File: sh.c
// Description: system shell
// Functions: getline, malloc, realloc, free, strtok, strlen, strcpy, printf
// Problems: leaks may be possible with full_path variable and concat function



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char **exec_input;

// Help functions
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    if (result == NULL){
        printf("Error: Failed to concatenate strings");
        exit(0);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}



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
    if (exec_input == NULL){
        printf("Error: Failed to allocate space for tokens");
        exit(0);
    }
    token = strtok(input, delimeter);
    for(i = 0; token != NULL; i++) {
        // Allocation and error detection
        exec_input[i] = malloc(sizeof(char) * (strlen(token)+1));
        if (exec_input[i] == NULL){
            printf("Error: Failed to allocate space for the token");
            exit(0);
        }

        strcpy(exec_input[i], token);

        // Reallocation and error detection
        exec_input = realloc(exec_input, sizeof(char*) * ++buff_size);
        if (exec_input == NULL){
            printf("Error: Failed to reallocate space for the next token");
            exit(0);
        }
        
        token = strtok(NULL, delimeter);
    }
    exec_input[i] = NULL;
    i = 0;
    
    // DEBUG PRINT STATEMENTS
    // printf("\n\n\n");
    // for (i = 0; exec_input[i] != NULL; i++)
    //     printf("%s\n", exec_input[i]);
}

int main(){
    int i, j;               
    int pid2, pid2_wait;
    
    char* check_buffer = NULL;
    char* input_buffer = NULL;
    size_t size = 0;

    char* token;
    char delimeter[2] = ":";
    char* full_path;
    char include_paths[] = "/usr/bin/:/usr/sbin/";

    while(1){
        // Get input
        printf("\n> ");
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

        // Create a new procces for the binary executable
        pid2 = fork();
        if (pid2 < 0){
            printf("Error: failed to start a new proccess");
            exit(0);
        }
        // Execute the program
        else if (pid2 == 0){
            if (access(exec_input[0], X_OK) == -1){
                token = strtok(include_paths, delimeter);
                for (i = 0; token != NULL; i++){
                    full_path = concat(token, exec_input[0]);

                    // Try to execute binary in the using include dirs
                    if (access(full_path, X_OK) == 0)
                        execv(full_path, exec_input);

                    free(full_path);
                    token = strtok(NULL, delimeter);
                }      
                printf("Error: command not found or you do not have access to it\n");
                exit(0);        
            }

            // Execute the user specified binary without include dirs
            execv(exec_input[0], exec_input);
        }
        else
            pid2_wait = wait(NULL);
            
        // Free buffers
        for (i = 0; exec_input[i] != NULL; i++)
                free(exec_input[i]);
        free(exec_input);
    }
    
    free(input_buffer);
    return 0;
}

