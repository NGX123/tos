#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testw();
void errorCorrect();
// File: addressbook.c
// Description: an addressbook to be added to the custom unix project
// Functions: fopen, fread, fwrite, fclose      printf, scanf, strcmp      exit, malloc, realloc, free 

// Defines and declarations
struct contact {
    char name[15];
    char surname[15];
    char phone[13];
    char email[20];
    char description[20];
};
struct contact* buffer;
int amount;




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

    buffer = malloc(sizeof(struct contact) * amount);
    
    if (buffer == NULL){
        printf("Failed to allocate memory\n");
        return -1;
    }

    if (fread(buffer, sizeof(struct contact), amount, fp) == -1){
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

    if (fwrite(buffer, sizeof(struct contact), amount, fp) == -1){
        printf("Failed to write buffer");
        exit(1);
    }

    if (fclose(fp) != 0) { 
        printf("Failed to close the file\n");
        exit(1);
    }
}





// Searches for the attribute through contacts
void search(){
    int option;
    printf("\nWhat attribute of the contact should I search for: \n\
    1. Name         \n\
    2. Surname      \n\
    3. Phone        \n\
    4. Email        \n\
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
            check = strcmp(buffer[i].surname, str);
            break;

        case 3:
            check = strcmp(buffer[i].phone, str);
            break;

        case 4:
            check = strcmp(buffer[i].email, str);
            break;

        default:
            printf("Wrong option");
            break;
    }
        if (check == 0){
            printf("Contact Description \n\
            Name - %s                   \n\
            Surname - %s                \n\
            Phone - %s                  \n\
            Email - %s                  \n\
            Description - %s            \n", buffer[i].name, buffer[i].surname, buffer[i].phone, buffer[i].email, buffer[i].description);
            is = 1;
            break;
        }         
    }
    if(is == 0) printf("No contact with entered attribute\n");
}


// Adds contacts to the file
void addContact(){
    printf("To add contact enter all the information needed\n");
    amount++;
    char tmpStr[20];
    buffer = realloc(buffer, sizeof(struct contact) * amount);

    if (buffer == NULL){
        printf("Failed to open the file\n");
        exit(1);
    }
    
    // COULD CONTAIN ERROR - change swithc back to else if
    for (int i=0; i <= 5; i++)
        switch(i){
            case 1:
                printf("What is the name: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].name, tmpStr);
                break;
            
            case 2:
                printf("\nWhat is the surname: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].surname, tmpStr);
                break;

            case 3:
                printf("\nWhat is the phone number: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].phone, tmpStr);
                break;

            case 4:
                printf("\nWhat is the email: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].email, tmpStr);
                break;
            
            case 5:
                printf("\nDescription: ");
                scanf("%s", tmpStr);
                strcpy(buffer[amount-1].description, tmpStr);
                break;
        }
}


// Removes contacts from the file
void removeContact(){
    char forremoval[20];
    int i;

    printf("Choose a contact to remove: ");
    scanf("%s", forremoval);

    for (i = 0; i < amount; i++){
        int check = strcmp(buffer[i].name, forremoval);
        if (check == 0)
            break;
    }
    printf("Succesfully removed - %s", buffer[i].name);

    for (; i < amount; i++)
        buffer[i] = buffer[i+1];
    
    amount -= 1;
    buffer = realloc(buffer, sizeof(struct contact) * amount);
}


// List contacts
void listContacts(){
    printf("Current amount of contacts: %d\n", amount);
    for (int i = 0; i < amount; i++)
        printf("%s\n", buffer[i].name);
}




// Creates a new file in case inputted in main() does not exist
void newFile(char filename[]){
    printf("It looks like the file does not exit, new file created\n");

    FILE *tmpFile = fopen(filename, "wb");
    fclose(tmpFile);

    if (readf(filename) == -1){
        printf("Failed to open the file\n");
        exit(1);
    }
        
}


int main(){
    int option;
    int run = 1;
    char filename[20];

    printf("Welcome to the address book app, you will be prompted to type information about the contat and it will store it in a file that coud also be read by it\n");
    printf("Type the name of the file with contacts: ");
    scanf("%s", &filename);
    if (readf(filename) == -1)
        newFile(filename);
    

    while(run){
        printf("\nPick an action:    \n\
        1. Quit                      \n\
        2. Search/Open               \n\
        3. Add                       \n\
        4. Remove                    \n\
        5. List                      \n\
        ");
        scanf("%d", &option);


        if (option == 1)
            break;
        else if (option == 2)
            search();
        else if (option == 3)
            addContact();
        else if (option == 4)
            removeContact();
        else if (option == 5)
            listContacts();
        // else if (option == 7)
        //     testw();
        // else if (option == 5)
        //     errorCorrect();
        else
            printf("Wrong option selected");
    }


    writef(filename);
    free(buffer);
    return 0;
}


// TEST funcs
void testw(){
    int num = 1;
    struct contact test = {0};
    strcpy(test.name, "Volodimir");
    strcpy(test.surname, "Ilich");
    strcpy(test.phone, "0502804541");
    strcpy(test.email, "ytx@ck.su");

    FILE* fp = fopen("contacts.cts", "wb");

    printf("testw - %s\n", test.name);

    if (fp == NULL){
        printf("File can not be opened\n");
        exit(1);
    }

    if (fwrite(&num, sizeof(num), 1, fp) == -1){
        printf("Failed to write number to the file\n");
        exit(1);
    }

    if (fwrite(&test, sizeof(test), num, fp) == -1){
        printf("Failed to write struct to the file\n");
        exit(1);
    }

    if (fclose(fp) != 0) { 
        printf("Failed to close the file\n");           /// HERE WAS THE PROBLEM - ALWAYS CLOSE FILES
        exit(1);
    }
}

// Erorr correction
void errorCorrect(){
    printf("Number of contacts: ");
    scanf("%d", &amount);
}