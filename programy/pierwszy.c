#include <stdio.h>

int licz(int a, int b);

void main()
{
    int a = 3;

    if(a % 2 == 0)
    {
        printf("parzysta\n");
    }
    else
    {
        printf("nieparzysta\n");
    }
    

    printf("Liczba %d liczba 245 %d daje wynik %d", 1, 2, licz(1,2) * licz(1,2));
} 

int licz(int a, int b)
 {
     return a+b;
 }