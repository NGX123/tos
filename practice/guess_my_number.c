// A computer selects random number using given settings and you guess it

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Prints a random number in range
int genrand(int lower, int upper) {  
    int num = (rand() % (upper - lower + 1)) + lower; 
    return num; 
} 

// The core loop of the game
int game(int tries, int secret){
    int run = 1;
    int tries_used = 1;
    int input;

    while (run){
        if (tries_used > tries){
            printf("You used all of available tries");
            break;
        }
            
        printf(">");
        scanf("%d", &input);
        if (input > 100 || input < 0)
            printf("Number is out of range\n");
        else {
            if (input == secret){
                printf("You won from %d tries", tries_used);
                break;
            }
            else {
                if (input < secret)
                    printf("The number is bigger\n");
                else 
                    printf("Number is smaller\n");
            }    
        }
        tries_used++;
    }
}



int main(){
    // Variables
    int tries;
    int upper;
    int secret;
    int lower = 0;
    
    

    // Settings
    printf("Choose amount of tries: ");
    scanf("%d", &tries);

    printf("\nChoose the range of number: ");
    scanf("%d", &upper);
    printf("\n");

    srand(time(0));
    secret = genrand(lower, upper);


    game(tries, secret);
}