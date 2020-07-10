#include <stdio.h>
#include <string.h>
int main(){
    int i, j;
    char string[40];
    char copystring[40];
   

    scanf("%s", string);
    int stringlen = strlen(string);
    for (i = 0, j = (stringlen-1); i <= stringlen && j >= 0; i++, j--){
        copystring[i] = string[j];
    }
    
    copystring[i] = '\0';
    if (strcmp(string, copystring) == 0)
        printf("String is a polindrom\n");
}