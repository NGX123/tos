#include <stdio.h>

int main(){
    int a, b, c;
    scanf("%d", &b);

    a = b - 1;
    for (int i = 0; i <= 10; i++){
        c = b;
        b = a + b;
        a = c;
        printf("%d\n", b);
    }
}