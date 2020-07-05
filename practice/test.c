#include <stdio.h>
void displayCalendar(){
    int flag = 0, k = 0;
    char date[12], day[3], month[3], year[5];
    printf("Input the date for calendar to be displayed in format dd-mm-yyyy: ");
    scanf("%s", date);
    printf("%s\n", date);

    for (int i = 0; i < 12; i++){
        if (date[i] == '-'){
            flag++;
            k = 0;
        }
            

        if (flag == 0 && date[i] != '-'){
            day[k++] = date[i];
            printf("Flag %d\n", flag);
        }

        else if (flag == 1 && date[i] != '-'){
            month[k++] = date[i];
            printf("Flag %d\n", flag);
        }

        else if (flag == 2 && date[i] != '-'){
            year[k++] = date[i];
            printf("Flag %d\n", flag);
        }
    }
    day[2] = '\0';
    month[2] = '\0';
    year[5] = '\0';
    printf("Day - %s, Month - %s, Year - %s", day, month, year);
}

int main(){
    displayCalendar();
}