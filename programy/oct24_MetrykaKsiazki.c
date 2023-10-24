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

element* utworz_element(element* glowa, metryka_ksiazki ksiazka)
{
    if(glowa)
    {
        if (glowa -> nastepny)
            return utworz_element(glowa->nastepny, ksiazka);
        else
        {
            glowa->nastepny = malloc(sizeof(element));
            *(glowa->nastepny) = (element) { .ksiazka = ksiazka, .nastepny = NULL};
            return glowa;
        }
    }
    else
    {
        element* ptr = malloc(sizeof(element));
        *ptr = (element) { .ksiazka = ksiazka, .nastepny = NULL };
        return ptr;
    }
}

void main()
{
    element* glowa = NULL;

    glowa = utworz_element(glowa, (metryka_ksiazki) { .tytul = "Przygody Kubusia Puchatka", .autor = "A. A. Milne", .rok_wydania = 1929, .liczba_stron = 124, .cena = 36.7});

    glowa = utworz_element(glowa, (metryka_ksiazki) { .tytul = "Dziwne przypadki Prosiaczka", .autor = "A. A. Milne", .rok_wydania = 1932, .liczba_stron = 87, .cena = 66.32});

    element *wypisywana = glowa;
    while (wypisywana)
    {
        printf("Tytul: %s\n", wypisywana->ksiazka.tytul);
        printf("Autor: %s\n", wypisywana->ksiazka.autor);
        printf("Rok wydania: %d\n", wypisywana->ksiazka.rok_wydania);
        printf("Liczba stron: %d\n", wypisywana->ksiazka.liczba_stron);
        printf("Cena: %.2f PLN\n", wypisywana->ksiazka.cena);
        printf("\n");

        wypisywana = wypisywana->nastepny;
    }
}
