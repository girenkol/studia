#include <stdio.h>
#include <malloc.h>

#ifndef LISTA_2_KIERUNKOWA

#define LISTA_2_KIERUNKOWA

typedef struct element
{
    int liczba;
    struct element *nastepny, *poprzedni;
} element;

element* dodaj(element* glowa, int liczba)
{
    if(glowa)
    {
        element* glowa_startowa = glowa;
        while(glowa->nastepny)//idzie do pustego
            glowa=glowa->nastepny;

        glowa->nastepny = malloc(sizeof(element));
        *(glowa->nastepny) = (element) { .liczba = liczba, .nastepny = NULL, .poprzedni = glowa->poprzedni};
        return glowa_startowa;
    }
    else
    {
        element* glowa_startowa = malloc(sizeof(element));
        *glowa_startowa = (element) { .liczba = liczba, .nastepny = NULL, .poprzedni = NULL};
        return glowa_startowa;
    }
    //*nowy = (element) { .liczba = liczba, .nastepny=NULL, .poprzedni=glowa};
}

int dlugosc(element *glowa)
{
    if(!glowa) return 0;
    int i=1;
    for(; glowa->nastepny; i++, glowa=glowa->nastepny); 
    return i;
}

element *usun(element *glowa, int indeks) //zmienic potem zeby zerowy byl pierwszym
{
    element* glowa_startowa = glowa;
    if (glowa)
    {
        element *poprzedni = NULL;
        for(int i = 0; i < indeks && glowa; i++, poprzedni = glowa, glowa = glowa->nastepny);
        if (poprzedni)
            poprzedni->nastepny = glowa->nastepny;
        else
            glowa = glowa->nastepny;
        free(glowa);
    }
    return glowa_startowa;
}

element *wstaw_przed(element *glowa, int indeks, void* dane);
element *wstaw_po(element *glowa, int indeks, void* dane);
void zamien(element *glowa, int indeks_1, int indeks_2);
void sortuj(element *glowa);

void zniszcz(element* glowa) //niszczarka cos nie wspolpracuje, 
{
    if(glowa)
    {
        zniszcz(glowa->nastepny);
        free(glowa);
    }
    glowa = NULL;
}

void wypisz(element* glowa)
{
    if(glowa)
    {
        printf("liczba w elemencie: %d\n", glowa->liczba);
        wypisz(glowa->nastepny);
    }     
    else
        printf("TO WSZYSTKO\n");
}

void main()
{
    element* glowa = NULL;
    glowa = dodaj(glowa, 13);
    glowa = dodaj(glowa, 19);
    glowa = dodaj(glowa, 43);
    glowa = dodaj(glowa, 102);
    wypisz(glowa);

    usun(glowa, 1);

    wypisz(glowa);
    
    printf("dlugosc: %d\n", dlugosc(glowa));

    zniszcz(glowa);
    wypisz(glowa);
}

#endif
