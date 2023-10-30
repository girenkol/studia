#include <stdio.h>
#include <malloc.h>

typedef struct element
{
    int liczba;
    struct element* nastepny;
    struct element* poprzedni;
} element;

int ilosc (element* glowa)
{
    if (!glowa)
    return 0;
    int d=1;
    while (glowa->nastepny)
    {
        glowa=glowa->nastepny;
        d++;
    }
    return d;
}

element* nowy (element* glowa, int liczba)
{
    if (glowa)
    {
        if (glowa->nastepny)
        {
            nowy(glowa->nastepny, liczba);
        }
        else
        {
            glowa->nastepny=malloc(sizeof(element));
            *(glowa->nastepny) = (element) {.liczba = liczba, .nastepny=NULL, .poprzedni=glowa};
        }
        return glowa;
    }
    else
    {
        element* pom =malloc(sizeof(element));
        *pom = (element) {.liczba = liczba, .nastepny=NULL, .poprzedni=NULL};
        return pom;
    }
}

element* nowyfor (element* glowa, int liczba)
{
    if (glowa)
    {
        element* ptr = glowa;
        while (glowa->nastepny)
        {
            glowa=glowa->nastepny;
        }
        glowa->nastepny=malloc(sizeof(element));
        *(glowa->nastepny) = (element) {.liczba = liczba, .nastepny=NULL, .poprzedni=glowa};
        return ptr;
    }
    else
    {
        element* pom=malloc(sizeof(element));
        *pom = (element) {.liczba = liczba, .nastepny=NULL, .poprzedni=NULL};
        return pom;
    }
}

element* nowyi (element* glowa, int liczba, int miejsce)
{
    if (miejsce>ilosc(glowa))
    {
        glowa=nowyfor(glowa, liczba);
        return glowa;
    }
    if (miejsce<1)
    return glowa;
    /*if (miejsce==1)
    {
        element* pom=malloc(sizeof(element));
        *pom= (element) {.liczba=liczba, .poprzedni=NULL, .nastepny=glowa};
        glowa->poprzedni=pom;
        return pom;
    }*/
    if (miejsce>1)
    {
        element* ptr=glowa, *hum=malloc(sizeof(element));
        for (int i=2; i<miejsce; i++)
        {
            glowa=glowa->nastepny;
        }
        *hum = (element) {.liczba=liczba, .nastepny=glowa->nastepny, .poprzedni=glowa};
        glowa->nastepny->poprzedni=hum;
        glowa->nastepny=hum;
        return ptr;
    }
}

void pokaz (element* glowa)
{
    if (glowa)
    {
        printf("liczba to %d\n", glowa->liczba);
        pokaz (glowa->nastepny);
    }
    return;
}

element* usunkoniec (element* glowa)
{
    if (!glowa)
    return NULL;
    if (glowa->nastepny)
    {
        element* pom=glowa;
        while (glowa->nastepny->nastepny)
        {
            glowa=glowa->nastepny;
        }
        free(glowa->nastepny);
        glowa->nastepny=NULL;
        return pom;
    }
    else
    {
        free(glowa);
        return NULL;
    }
}

element* usuni (element* glowa, int miejsce)
{
    if (miejsce>=ilosc(glowa))
    {
        glowa=usunkoniec(glowa);
        return glowa;
    }
    if (miejsce<1)
    return glowa;
    if (miejsce==1)
    {
        element* pom=glowa->nastepny;
        free(glowa);
        pom->poprzedni=NULL;
        return pom;
    } 
    if (miejsce>1)
    {
        element* ptr=glowa;
        for (int i=2; i<miejsce; i++)
        {
            glowa=glowa->nastepny;
        }
        element* hum=glowa->nastepny->nastepny;
        free(glowa->nastepny);
        glowa->nastepny=hum;
        hum->poprzedni=glowa;
        return ptr;
    }
}

element* zamien (element* glowa, int miejsce1, int miejsce2)
{
    {
    if (ilosc(glowa)<2)
    return glowa;
    if (miejsce1>ilosc(glowa))
    miejsce1=ilosc(glowa);
    if (miejsce2>ilosc(glowa))
    miejsce2=ilosc(glowa);
    if (miejsce1==miejsce2)
    return glowa;
    if (miejsce1>miejsce2)
    {
        int k=miejsce1;
        miejsce1=miejsce2;
        miejsce2=k;
    }
    }
    element* ptr=glowa;
    for (int i=1; i<miejsce1; i++)
    {
        glowa=glowa->nastepny;
    }
    element* pierw=glowa, *pom1=glowa->nastepny, *pom2=glowa->poprzedni;
    for (int i=miejsce1; i<miejsce2; i++)
    {
        glowa=glowa->nastepny;
    }
    element* hum1=glowa->nastepny, *hum2=glowa->poprzedni;
    if (pom2)
    {
        pom2->nastepny=glowa;
    }
    else
    {
        ptr=glowa;
    }
    glowa->poprzedni=pom2;
    pierw->nastepny=hum1;
    if(hum1)
        hum1->poprzedni=pierw;
    if(hum2==pierw)
    {
        glowa->nastepny=hum2;
        hum2->poprzedni=glowa;
    }
    else
    {
        glowa->nastepny=pom1;
        pom1->poprzedni=glowa;
        hum2->nastepny=pierw;
        pierw->poprzedni=hum2;
    }
    return ptr;
}

void main ()
{
    element* glowa=NULL;
    pokaz (glowa);
    glowa=nowyfor(glowa, 1); 
    glowa=nowyfor(glowa, 2);
    glowa=nowyfor(glowa, 3);
    glowa=nowyfor(glowa, 4);
    pokaz (glowa);
    printf("ilosc elementow to %d\n",ilosc(glowa));
    glowa=nowyi(glowa, 120, 3);
    pokaz (glowa);
    printf("ilosc elementow to %d\n",ilosc(glowa));
    glowa=usuni(glowa, 3);
    pokaz (glowa);
    printf("ilosc elementow to %d\n",ilosc(glowa));
    glowa=nowyfor(glowa, 5);
    glowa=nowyfor(glowa, 6);
    glowa=zamien (glowa, 1, 5);
    pokaz (glowa);
}
