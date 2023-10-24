#include <stdio.h>
#include <malloc.h>

typedef struct 
{
    char tytul[30], autor[30];
    int rok_wydania, liczba_stron;
    float cena;
} metryka_ksiazki;

typedef struct element
{
    metryka_ksiazki ksiazka;
    struct element* nastepny;
} element;

void main()
{

}