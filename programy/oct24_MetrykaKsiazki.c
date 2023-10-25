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
    struct element* nastepny; //lista jednokierunkowa [glowa] -> [nastepny] itd.
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

element* usun_z_konca(element* glowa)
{
    if (!glowa) //pusta lista
        return;
    if(!glowa->nastepny) //jest tylko glowa
    {
        free(glowa);
        return;
    }
    //wiecej niz jeden element w liscie
    //[z projektora] for(; glowa->nastepny; glowa = glowa -> nastepny); // tu było drugie ->nastepny (po co?, ale nie dziala bez) for(; glowa->nastepny && glowa->nastepny->nastepny; glowa = glowa -> nastepny);
    
    while(glowa->nastepny && glowa->nastepny->nastepny) glowa = glowa -> nastepny;

    free(glowa->nastepny);
    glowa->nastepny = NULL;
}

void usun_z_konca_rekurencyjnie(element* glowa)
{
    if (!glowa) return;
    if (!glowa->nastepny)
        free(glowa);
    else
    {
        if (glowa->nastepny->nastepny) //jeżeli w kolejnym coś jest to przeskakuje do niego
            usun_z_konca_rekurencyjnie(glowa->nastepny);
        else //jeżeli to jest ostatni to go usuwa
        {
            free(glowa->nastepny);
            glowa->nastepny = NULL;
        }
        
    }
}

void usun_dany_element(element* glowa, int nr_elementu)
{
    if(!glowa) return;
    if (!glowa->nastepny)
        free(glowa);
    for (int i=0; glowa->nastepny && glowa->nastepny->nastepny; i++)
    {
        if(i == nr_elementu)
        {
            free(glowa->nastepny);
            glowa->nastepny = NULL;
        }
    }
    
}

void wypisz(element* glowa)
{
    element *wypisywana = glowa;
    while (wypisywana)
    {
        printf("Tytul: %s\n", wypisywana->ksiazka.tytul);
        printf("Autor: %s\n", wypisywana->ksiazka.autor);
        printf("Rok wydania: %d\n", wypisywana->ksiazka.rok_wydania);
        printf("Liczba stron: %d\n", wypisywana->ksiazka.liczba_stron);
        printf("Cena: %.2f zł\n", wypisywana->ksiazka.cena);
        printf("\n");

        wypisywana = wypisywana->nastepny;
    }
}

/*void wypisz(element* glowa) //to jest wypisywaie z projek ale dla programu z liczbami
{
    if (glowa)
    {
        printf("%p = %d\n", glowa->liczba);
        if (glowa->nastepny)
            wypisz(glowa->nastepny);
    }
}*/

void main()
{
    element* glowa = NULL;

    glowa = utworz_element(glowa, (metryka_ksiazki) { .tytul = "Przygody Kubusia Puchatka", .autor = "A. A. Milne", .rok_wydania = 1929, .liczba_stron = 124, .cena = 36.7});

    glowa = utworz_element(glowa, (metryka_ksiazki) { .tytul = "Dziwne przypadki Prosiaczka", .autor = "A. A. Milne", .rok_wydania = 1932, .liczba_stron = 87, .cena = 66.32});
    
    glowa = utworz_element(glowa, (metryka_ksiazki) { .tytul = "Puchatek na tropie Harryego Pottera", .autor = "Jaktak Jessing", .rok_wydania = 1966, .liczba_stron = 456, .cena = 70.55});

    wypisz(glowa);
    printf("Tworzenie elementow zakonczone\n\n");
    //usun_z_konca_rekurencyjnie(glowa); //ALBO usun_z_konca(glowa);

    usun_dany_element(glowa, 1);

    wypisz(glowa);
}
