#include <stdio.h>
#include <malloc.h>

typedef struct element //deklaracja struktury danych dla liczb zespolonych
{
    int re, im;
    struct element *nastepny, *poprzedni;
} element;

element* dodaj(element* glowa, int re, int im) //dodawanie elementu na koniec listy
{
    if(glowa) // jezeli nie jest pusta
    {
        element* glowa_startowa = glowa; //zapamietuje adres glowy zerowej
        while(glowa->nastepny) //idzie do pustego, zeby glowa byla na ostatnim elemencie
            glowa=glowa->nastepny;

        glowa->nastepny = malloc(sizeof(element)); //rezerwuje pamiec za ostatnim elementem
        *(glowa->nastepny) = (element) { .re = re, .im = im, .nastepny = NULL, .poprzedni = glowa->poprzedni}; //deklaruje jego dane
        return glowa_startowa; //zwraca spowrotem do glowy adres glowy zerowej
    }
    else // jezeli lista jest pusta
    {
        element* glowa_startowa = malloc(sizeof(element));
        *glowa_startowa = (element) { .re = re, .im = im, .nastepny = NULL, .poprzedni = NULL};
        return glowa_startowa;
    }
}

int dlugosc(element *glowa) // zwraca dlugosc listy
{
    element* glowa_startowa = glowa;
    if(!glowa) return 0; // jezeli glowa jest pusta to zwraca 0
    int i=1;
    for(; glowa->nastepny; i++, glowa=glowa->nastepny);
    glowa = glowa_startowa;
    return i; //zwraca ten licznik
}

element *usun(element *glowa, int indeks) // usuwa dany element listy
{
    element* glowa_startowa = glowa;
    if (glowa)
    {
        element *prev = NULL; //deklaruje adres poprzedniej komorki
        for(int i = 0; i < indeks && glowa; i++, prev = glowa, glowa = glowa->nastepny); // przesuwa glowe do adresu glowy o numerze indeks i jezeli podany indeks jest poza lista to przerywa petle, jednoczesnie zapamietuje adres poprzedniej komorki wzgledem sprawdzanej glowy
        if (prev)
            prev->nastepny = glowa->nastepny; // ata element przed usunietym z elementem po usunietym
        else
            glowa = glowa->nastepny; // jezeli zostal usuniety element zerowy to lata tylko z nastepnym
        glowa->nastepny->poprzedni = prev;
        free(glowa); // zwalnia pamiec usuwanego elementu
    }
    return glowa_startowa; // zwraca adres glowy zerowej
}

element *wstaw_po(element *glowa, int indeks, int re, int im)
{
    element *glowa_startowa = glowa; //zapisuje adres glowy zerowej i potem ja funkcja zwraca

    if(glowa)
    {
        int dlugosc_listy = dlugosc(glowa); // bierze dlugosc listy z funkcji dlugosc
        if (indeks >= dlugosc_listy)// jezeli indeks jest wiekszy lub równy dlugosci listy lub lista jest pusta to wywoluje funkcje dodaj zeby dodac na koncu
            glowa = dodaj(glowa, re, im); // dodaje na koncu
        else //jezeli lista zawiera elementy i podany indeks nie jest wiekszy od dlugosci listy
        {
            element* nowy = malloc(sizeof(element)); //to cale deklaruje wartosci nowego elementu potem na te miejsca przypisuje dane
            nowy->re = re;
            nowy->im = im;
            nowy->nastepny = NULL;
            nowy->poprzedni = NULL;

            if (indeks == 0) // Wstawienie na poczatku listy
            {
                nowy->nastepny = glowa; //laczenie komorek listy
                glowa->poprzedni = nowy;
                glowa_startowa = nowy; //i wpisanie w komorke zerowa
            }
            else
            {
                for (int i = 1; i < indeks && glowa; i++, glowa = glowa->nastepny); //utawia glowe na szukany indeks

                nowy->nastepny = glowa->nastepny; //laczy komorke na miejscu indeks z komorka nowy
                if (glowa->nastepny)
                {
                    glowa->nastepny->poprzedni = nowy; //i w druga strone
                }
                nowy->poprzedni = glowa; //laczy komorke na miejscu indeks z komórki nowy                    
                glowa->nastepny = nowy; //i w druga strone
            }
        }
    }
    else //dla pustej listy
    {
        element* glowa_startowa = malloc(sizeof(element));
        *glowa_startowa = (element) { .re = re, .im = im, .nastepny = NULL, .poprzedni = NULL};
        return glowa_startowa;
    }

    return glowa_startowa;
}

element *wstaw_przed(element *glowa, int indeks, int re, int im)
{
    element *glowa_startowa = glowa;

    if(glowa)
    {
        int dlugosc_listy = dlugosc(glowa); 
        if (indeks >= dlugosc_listy)
            glowa = dodaj(glowa, re, im); 
        else 
        {
            element* nowy = malloc(sizeof(element)); 
            nowy->re = re;
            nowy->im = im;
            nowy->nastepny = NULL;
            nowy->poprzedni = NULL;

            if (indeks == 0 || indeks == 1) 
            {
                nowy->nastepny = glowa; 
                glowa->poprzedni = nowy;
                glowa_startowa = nowy;
            }
            else
            {
                for (int i = 2; i < indeks && glowa; i++, glowa = glowa->nastepny);

                nowy->nastepny = glowa->nastepny;
                if (glowa->nastepny)
                {
                    glowa->nastepny->poprzedni = nowy; 
                }                    
                nowy->poprzedni = glowa; 
                glowa->nastepny = nowy; 
            }
        }
    }
    else //dla pustej listy
    {
        element* glowa_startowa = malloc(sizeof(element));
        *glowa_startowa = (element) { .re = re, .im = im, .nastepny = NULL, .poprzedni = NULL};
        return glowa_startowa;
    }

    return glowa_startowa;
}

element* zamien(element *glowa, int indeks_1, int indeks_2)
{
    element *glowa_poczatkowa = glowa;

    int dlugosc_listy = dlugosc(glowa);

    if (glowa && indeks_1 != indeks_2 && indeks_1 > 0 && indeks_2 > 0 && indeks_2 <= dlugosc_listy)
    {
        element *komorka_1 = glowa;
        element *komorka_2 = glowa;

        for (int i = 1; i < indeks_1; i++)
            komorka_1 = komorka_1->nastepny;

        for (int i = 1; i < indeks_2; i++)
            komorka_2 = komorka_2->nastepny;

        int re_bufor = komorka_1->re;
        int im_bufor = komorka_1->im;

        komorka_1->re = komorka_2->re;
        komorka_1->im = komorka_2->im;

        komorka_2->re = re_bufor;
        komorka_2->im = im_bufor;

        printf("\n kom1 %d | kom2 %d\n", komorka_1->re, komorka_2->re);
    }
    else
    {
        printf("Zamiana niemozliwa bo indeksy podane blednie lub lista jest pusta\n");
    }

    return glowa_poczatkowa;
}

element *zniszcz(element* glowa) // czysci i zwalnia pamiec calej listy
{
    if(glowa) //czy nie jest pusta
    {
        zniszcz(glowa->nastepny); // klika do konca
        free(glowa);
    }
    return NULL; // jak kliknie do konca to wraca do ifa i robi free
}

void wypisz(element* glowa) // wypisuje cala zarartosc glowy w postaci liczb zespolonych re + im i
{
    if(glowa)
    {
        printf("liczba w elemencie: %3d + %3d i \n", glowa->re, glowa->im);
        wypisz(glowa->nastepny); // klika rekurencyjnie do konca, tak samo jak w f zniszcz
    }
    else
        printf("TO WSZYSTKO\n"); // jak skonczy wypisywac to pisze ze juz
}

void main()
{
    element* glowa = NULL; //zadeklarowanie glowy

    glowa = dodaj(glowa, 13, 2);
    glowa = dodaj(glowa, 19, 1);
    glowa = dodaj(glowa, 1, 4);
    glowa = dodaj(glowa, 43, 54);
    glowa = dodaj(glowa, 102, 99); //wprowadzenie danych
    wypisz(glowa);
    printf(">> wpisano elementy do listy\n\n");

    glowa = usun(glowa, 3); //usuwan wybrany
    wypisz(glowa);
    printf("dlugosc: %d\n", dlugosc(glowa));
    printf(">> usunieto element z listy\n\n");

    glowa = wstaw_po(glowa, 2, 33, 23); //wstawia po wybranym
    wypisz(glowa);
    printf("po wstawieniu po dlugosc: %d\n", dlugosc(glowa));
    printf(">> wstawiono element funkcja wstaw_po\n\n");

    glowa = wstaw_przed(glowa, 2, 12, 12); //wstawia przed wybranym
    wypisz(glowa);
    printf("po wstawieniu przed dlugosc: %d\n", dlugosc(glowa));
    printf(">> wstawiono element funkcja wstaw przed\n\n");

    glowa = zamien(glowa, 1, 3); //swap
    wypisz(glowa);
    printf("check check\n");
    printf(">> zamieniono ze soba dwa elementy listy\n\n");

    glowa = zniszcz(glowa);
    wypisz(glowa);
    printf("dlugosc: %d\n", dlugosc(glowa));
    printf(">> zniszczono liste\n\n");
}
