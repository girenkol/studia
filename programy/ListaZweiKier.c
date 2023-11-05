#include <stdio.h>
#include <malloc.h>

#ifndef LISTA_2_KIERUNKOWA

#define LISTA_2_KIERUNKOWA

typedef struct element //teklaracja struktury danych dla liczb zespolonych
{
    int re, im;;
    struct element *nastepny, *poprzedni;
} element;

element* dodaj(element* glowa, int re, int im) //dodawanie elementu na koniec listy
{
    if(glowa) // jezeli nie jest pusta
    {
        element* glowa_startowa = glowa; //zapamietuje adres glowy zerowej
        while(glowa->nastepny)//idzie do pustego, zeby glowa byla na ostatnim elemencie
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
    //*nowy = (element) { .liczba = liczba, .nastepny=NULL, .poprzedni=glowa};
}

int dlugosc(element *glowa) // zwraca dlugosc listy
{
    if(!glowa) return 0; // jezeli glowa jest pusta to zwraca 0
    int i=1;
    for(; glowa->nastepny; i++, glowa=glowa->nastepny); // przesuwa adres glowy do tej ostatniej jednoczesnie zwiekszajac licznik, nie musi byc glowy poczotkowej bo zmiana adresu glowy nastepuje tylko w zakresie fora
    return i; //zwraca ten licznik
}

element *usun(element *glowa, int indeks) // usuwa dany element listy
{
    element* glowa_startowa = glowa;
    if (glowa)
    {
        element *poprzedni = NULL; //deklaruje adres poprzedniej komorki
        for(int i = 0; i < indeks && glowa; i++, poprzedni = glowa, glowa = glowa->nastepny); // przesuwa glowe do adresu glowy o numerze indeks i jezeli podany indeks jest poza lista to przerywa petle, jednoczesnie zapamietuje adres poprzedniej komorki wzgledem sprawdzanej glowy
        if (poprzedni)
            poprzedni->nastepny = glowa->nastepny; // ³ata element przed usunietym z elementem po usunietym
        else
            glowa = glowa->nastepny; // jezeli zostal usuniety element zerowy to lata tylko z nastepnym
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
        if (indeks >= dlugosc_listy)// jezeli indeks jest wiêkszy lub równy d³ugoœci listy lub lista jest pusta to wywoluje funkcje dodaj zeby dodac na koncu
            glowa = dodaj(glowa, re, im); // dodaje na koncu
        else //jezeli lista zawiera elementy i podany indeks nie jest wiekszy od dlugosci listy
        {
            element* nowy = malloc(sizeof(element)); //to cale deklaruje wartosci nowego elementu potem na te miejsca przypisuje dane
            nowy->re = re;
            nowy->im = im;
            nowy->nastepny = NULL;
            nowy->poprzedni = NULL;

            if (indeks == 0) // Wstawienie na pocz¹tku listy
            {
                nowy->nastepny = glowa; //laczenie komorek listy
                glowa->poprzedni = nowy;
                glowa_startowa = nowy; //i wpisanie w komorke zerowa
            }
            else
            {
                for (int i = 1; i < indeks && glowa; i++, glowa = glowa->nastepny); //utawia glowe na szukany indeks

                    nowy->nastepny = glowa->nastepny; //laczy komórkê na miejscu indeks+1 z komork¹ nowy
                    if (glowa->nastepny)
                    {
                        glowa->nastepny->poprzedni = nowy; //i w druga strone
                    }
                        nowy->poprzedni = glowa; //laczy komórkê na miejscu indeks z komórk¹ nowy
                        glowa->nastepny = nowy; //i w druga strone
            }
        }
    }
    else //dla pustej listy
    {
        //dodaj(glowa, re, im); z jakiegos powodu nie dziala z samym tym dla pustej listy
        element* glowa_startowa = malloc(sizeof(element));
        *glowa_startowa = (element) { .re = re, .im = im, .nastepny = NULL, .poprzedni = NULL};
        return glowa_startowa;
    }

    return glowa_startowa;
}

element *wstaw_przed(element *glowa, int indeks, void* dane);

/*element* zamien(element *glowa, int indeks_1, int indeks_2)
{
    element *glowa_poczatkowa = glowa;
    int dlugosc_listy = dlugosc(glowa);
    if(dlugosc_listy <= 1)
        printf("Zamina jest niemozliwa\n");
    else
    {
        for(int i=1; i < indeks_1 && glowa->nastepny; i++, glowa = glowa->nastepny);
        int re1 = glowa->re;
        int im1 = glowa->im;
        glowa = glowa_poczatkowa;

        for(int i=1; i < indeks_2 && glowa->nastepny; i++, glowa = glowa->nastepny);
        int re2 = glowa->re;
        int im2 = glowa->im;
        glowa = glowa_poczatkowa;

        for(int i=1; i < indeks_1 && glowa->nastepny; i++, glowa = glowa->nastepny);
        glowa->re = re1;
        glowa->im = im1;
        glowa = glowa_poczatkowa;

        for(int i=1; i < indeks_2 && glowa->nastepny; i++, glowa = glowa->nastepny);
        glowa->re = re2;
        glowa->im = im2;
        glowa = glowa_poczatkowa;
    }
    return glowa_poczatkowa;
}*/ //nieudana próba zrobienia tego nie tak jak sie powinno

/*element* zamien(element *glowa, int indeks_1, int indeks_2)
{
    element *glowa_poczatkowa = glowa;

    int dlugosc_listy = dlugosc(glowa);

    if(glowa)
    {
        if(indeks_1 > indeks_2) // zamienia indeksy jezeli 1>2
        {
            int bufor;
            bufor = indeks_1;
            indeks_1 = indeks_2;
            indeks_2 = bufor;
        }


        if(glowa <= 0 || indeks_1 == indeks_2 || indeks_2 > dlugosc_listy)
        {
            printf("indeksy zostaly podane blednie\n");
            return glowa_poczatkowa;
        }
        else
        {
            element *komorka_1 = malloc(sizeof(element));
            element *komorka_2 = malloc(sizeof(element));


            element *bufor = malloc(sizeof(element));


            for(int i=1; i < indeks_1; i++, glowa=glowa->nastepny);
            komorka_1 = glowa;
            glowa = glowa_poczatkowa;

            bufor->re=komorka_1->re;
            bufor->im=komorka_1->im;
            bufor->nastepny=komorka_1->nastepny;
            bufor->poprzedni=komorka_1->poprzedni;

            for(int i=1; i < indeks_2; i++, glowa=glowa->nastepny);
            komorka_2 = glowa;
            glowa = glowa_poczatkowa;

            komorka_1->re=komorka_2->re;
            komorka_1->im=komorka_2->im;
            komorka_1->poprzedni=komorka_2->poprzedni;
            komorka_1->nastepny=komorka_2->nastepny;

            komorka_2->re=bufor->re;
            komorka_2->im=bufor->im;
            komorka_2->poprzedni=bufor->poprzedni;
            komorka_2->nastepny=bufor->nastepny;

            free(bufor);
            printf("\n %d | %d\n", komorka_1->re, komorka_2->re);
        }
    }
    else
    {
        printf("nie ma co zamieniac bo glowa jest pusta\n");
    }


    return glowa_poczatkowa;
}*/

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
        printf("Zamiana niemozliwa: indeksy podane blednie lub lista jest pusta.\n");
    }

    return glowa_poczatkowa;
}

void sortuj(element *glowa);

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

    glowa = usun(glowa, 3); //usuwan wybrany
    wypisz(glowa);
    printf("dlugosc: %d\n", dlugosc(glowa));

    glowa = wstaw_po(glowa, 2, 33, 23); //wstawia po wybranym
    wypisz(glowa);
    printf("po wstawieniu po dlugosc: %d\n", dlugosc(glowa));

    glowa = zamien(glowa, 1, 3); //robi swapa
    wypisz(glowa);
    printf("check check\n\n");

    glowa = zniszcz(glowa);
    wypisz(glowa);
    printf("dlugosc: %d\n", dlugosc(glowa));
}

#endif
