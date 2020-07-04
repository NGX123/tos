// File: calendar.c
// Description: calendar with support for events and other extra features for the custom UNIX project
// Functions: fopen, fread, fwrite, fclose      printf, scanf, strcmp      exit, malloc, realloc, free 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Defines
#define MAIN_MENU 1
#define YEAR 2
#define MONTH 3
#define DAY 4

// Declarations
struct eventDay {
    char name[20];
    char tag[20];
    char start_time[7];
    char end_time[7];
    char day;
    char weekDay[10];
    char month[10];
    int year;

};
struct eventDay* buffer;
int amount;
int location;





/// FILE I/O ///
// Reads the contents of the file into buffer
int readf(char filename[]){
    FILE* fp = fopen(filename, "rb");
    
    if (fp == NULL){
        printf("Failed to open the file\n");
        return -1;
    }

    if (fread(&amount, sizeof(amount), 1, fp) == -1){
        printf("Failed to read num from the file");
        return -1;
    }

    buffer = malloc(sizeof(struct eventDay) * amount);
    
    if (buffer == NULL){
        printf("Failed to allocate memory\n");
        return -1;
    }

    if (fread(buffer, sizeof(struct eventDay), amount, fp) == -1){
        printf("Failed to read struct from the file\n");
        return -1;
    }
    
    if (fclose(fp) != 0) { 
        printf("Failed to close the file\n");
        return -1;
    }
}

// Writes buffer back to the file
void writef(char filename[]){
    FILE* fp = fopen(filename, "wb");

    if (fp == NULL){
        printf("File can not be opened for writing");
        exit(1);
    }

    if (fwrite(&amount, sizeof(amount), 1, fp) == -1){
        printf("Writing amount failed");
        exit(1);
    }

    if (fwrite(buffer, sizeof(struct eventDay), amount, fp) == -1){
        printf("Failed to write buffer");
        exit(1);
    }

    if (fclose(fp) != 0) { 
        printf("Failed to close the file\n");
        exit(1);
    }
}

// Creates a new file in case it does not yet exist
void newFile(char filename[]){
    printf("The file with the name you inputted does not exit, it will be created\n");
    FILE* fp = fopen(filename, "w");

    if (fp == NULL){
        printf("Failed to make a new file\n");
        exit(1);
    }

    if (readf(filename) == -1){
        printf("Faile to read the generated file\n");
        exit(1);
    }

    fclose(fp);
}





/// DATE FUNCTIONS ///
// Finds out day of the week
int weekDay(int Day, int Month,int Year)
{
    int ZMonth, ZYear, WeekDay;
    if(Month < 3)
        ZMonth = Month+10;
    else
        ZMonth = Month-2;
    if(ZMonth > 10)
        ZYear = Year-1;
    else
        ZYear = Year;
    WeekDay = ((int)((13 * ZMonth - 1) / 5) + Day + ZYear % 100 + (int)((ZYear % 100) / 4) - 2 * (int)(ZYear / 100) + (int)(ZYear / 400) + 77) % 7;
    return WeekDay;
}

// Converts month from their character name to their number
int month_converter(char month){
    
}

// Return a list of days suitable for the calendar interface function 
int monthDays(char date[]){

}





/// EVENT FUNCTIONS ///
// Add an event
void addEvent(){
    amount++;
    char tmpStr[20];
    buffer = realloc(buffer, sizeof(struct eventDay) * amount);

    // COULD CONTAIN ERROR - change swithc back to else if
    for (int i=0; i <= 8; i++)
        switch(i){
            case 1:
                printf("What is the name: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].name, tmpStr);
                break;
            
            case 2:
                printf("\nWhat is the start time of the event: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].start_time, tmpStr);
                break;

            case 3:
                printf("\nWhat is the end time of the event: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].end_time, tmpStr);
                break;

            case 4:
                printf("\nWhat is the tag: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].tag, tmpStr);
                break;
            
            case 5:
                printf("\nWhat is the day of the event: ");
                scanf("%s", &buffer[amount-1].day);
                break;

            case 6:
                printf("\nWhat is the month of the event: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].month, tmpStr);
                break;

            case 7:
                printf("\nWhat is the year of the event: ");
                scanf("%d", &buffer[amount-1].year);
                break;

            case 8:
                printf("\nWhat is the weekday of the event: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].weekDay, tmpStr);
                break;
        }
}

// Remove an event
void removeEvent(){
    int i;
    char forremoval[20];

    printf("Type the name of the event for removal\n");
    scanf("%s", forremoval);

    for (int i = 0; i < amount; i++)
        if (strcmp(forremoval, buffer[i].name) == 0)
            break;

    for (; i < amount; i++)
        buffer[i] = buffer[i+1];
    
    amount --;
    buffer = realloc(buffer, sizeof(struct eventDay) * amount);  
    printf("Succesfully removed - %s", buffer[i].name);
}

// Search by tag or other attributes
void searchEvent(){
    int option;
    printf("\nWhat attribute of the contact should I search for: \n\
    1. Name         \n\
    2. Tag          \n\
    ");
    scanf("%d", &option);

    char str[15];
    printf("Input atrribute content to search for: ");
    scanf("%s", str);

    int check;
    int i;
    int is = 0;

    for (i = 0; i < amount; i++){
        switch(option){
        case 1:
            check = strcmp(buffer[i].name, str);
            break;

        case 2:
            check = strcmp(buffer[i].tag, str);
            break;

        default:
            printf("Wrong attribute selected");
            break;
        }   

        if (check == 0){
            printf("Contact Description\n\
            Name -          %s         \n\
            Tag -           %s         \n\
            Start time -    %s         \n\
            End time -      %s         \n\
            Day -           %c         \n\
            Week Day -      %s         \n\
            Month -         %s         \n\
            Year -          %d         \n", buffer[i].name, buffer[i].tag, buffer[i].start_time, buffer[i].end_time, buffer[i].day, buffer[i].weekDay, buffer[i].month, buffer[i].year);
            is = 1;
            break;
        }         
    }
    if(is == 0) printf("No contact with entered attribute\n");
}

// Lists the events - options to list in current month, year, week or all
int listEvents(){
    for (int i = 0; i < amount; i++){
        printf(buffer[i].name);
    }
}





/// CALENDAR INTERFACE ///
void displayCalendar(){
    int flag = 0, k = 0;
    char date[12], day[3], month[3], year[5];
    printf("Input the date for calendar to be displayed in format dd-mm-yyyy: ");
    scanf("%s", date);

    for (int i = 0; i < 12; i++){
        if (date[i] == '-'){
            flag++;
            k = 0;
        }
            

        if (flag == 0 && date[i] != '-'){
            day[k] = date[i];
            k++;
        }

        else if (flag == 1 && date[i] != '-'){
            month[k] = date[i];
            k++;
        }

        else if (flag == 2 && date[i] != '-'){
            year[k] = date[i];
            k++;
        }
    }
    day[2] = '\0';
    month[2] = '\0';
    printf("Day - %s, Month - %s, Year - %s", day, month, year);
}





int main(){
    char filename[20];
    int run = 1;
    int option;

    printf("This is an improved version of a calendar with features like making event and manageing them, going into nearly any place in time down to 1.1.2000 and up to 1.1.4000\n");
    printf("Input a name of your calendar file: ");
    scanf("%s", filename);
    if (readf(filename) == -1)
        newFile(filename);

    // Problemn indentation
    while (run){
        printf("\nPick an action:    \n\
        1. Quit                      \n\
        2. Search/Open               \n\
        3. Add                       \n\
        4. Remove                    \n\
        5. List                      \n\
        6. Display Calendar(DO NOT USE)\n\
        >  \
        ");
        scanf("%d", &option);


        switch(option){
            case 1:
            run = 0;
                break;
            case 2:
                searchEvent();
                break;
            case 3:
                addEvent();
                break;
            case 4:
                removeEvent();
                break;
            case 5:
                listEvents();
                break;
            case 6:
                displayCalendar();
                break;
            default:
                printf("Wrong option selected");
                break;
        }
    }
    
    writef(filename);
    free(buffer);
    return 0;
}

     

