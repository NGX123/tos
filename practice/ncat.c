// File: ncat.c
// Description: custom cat program for NUXOS
// Functions: fopen, fgetc, fclose      printf, strcmp      exit
/* Problems: 
    find a better way to implement flags(e.g -t)
    add flags - b, e, -s, -t 
    fix -n flag to print line number one too and number others sequentialy
    fix input of same flags like -A and -v*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int in(char element[], char *list[], int argc){
    for (int i = 0; i < argc; i++)
        if (strcmp(element, list[i]) == 0)
            return 0;
    return 1;
}

// Display file contents
void output(int argc, char *argv[]){
    FILE* fp;
    char filechar;
    int number_flag = 0;
    int counter = 1;

    for (int i = 1; i < argc; i++){
        fp = fopen(argv[i], "r");
        
        if (argv[i][0] == '-')
            continue;
        
        if (fp == NULL){
            printf("ERROR:\nFailed to open the file");
            exit(1);
        }

        while ((filechar = fgetc(fp)) != EOF){
            if (in("-A", argv, argc) == 0){
                if (filechar == '\n')
                    printf("$");
                if (filechar == '\t')
                    printf("^I");
            }

            if (in("-E", argv, argc) == 0)
                if (filechar == '\n')
                    printf("$");

            if (in("-n", argv, argc) == 0){
                if (filechar == '\n'){
                    number_flag = 1;
                    
                }
                    
                if (number_flag == 1 && filechar != '\n'){
                    number_flag = 0;
                    printf("%d. ", counter);
                }

                if (counter == 1){
                    printf("1. ");
                    counter++;
                }   
            }

            if (in("-T", argv, argc) == 0)
                if (filechar == '\t')
                    printf("^I");


            printf("%c", filechar);
        }
        
        fclose(fp);   
    }
}

int main(int argc, char **argv){
    // Check if arguments are supplied
    if (argc <= 1){
        printf("ERROR:\nNo arguments were given");
        exit(1);
    }

    if (in("--help", argv, argc) == 0){
        printf(" ");
    }
    output(argc, argv);
    

}
