#include<stdio.h>
#include<malloc.h>

/*typedef struct zespolona
{
    float re, im;
} zespolona;*/
typedef struct element
{
    int liczba;
    struct element* nastepny;
    struct element* poprzedni;

} element;

int ileichjest(element*glowa)
{
    if(!glowa)
    return 0;
    int i=1;
    while(glowa->nastepny)
    {
        glowa=glowa->nastepny;
        i++;
    }
    return i;   
         /*for(int i=1; i<ileichjest(glowa)&&glowa->nastepny; glowa=glowa->nastepny, i++)
    
        return i;*/
}

element* nowy(element* glowa, int liczba)
{
    if(glowa)
    {
       element*wskaznikpocz=glowa;
       while(glowa->nastepny)
       {
           glowa=glowa->nastepny;
       }
    glowa->nastepny=malloc(sizeof(element));
    *(glowa->nastepny)=(element){.liczba=liczba, .nastepny=NULL, .poprzedni=glowa};
    return wskaznikpocz;
    }
    else
    {
        element*pustaglowa=malloc(sizeof(element));
        *pustaglowa=(element){.liczba=liczba, .nastepny=NULL, .poprzedni=NULL};
    return pustaglowa;
    }
}

element* usunostatniego(element* glowa)
{
if(!glowa)
return NULL;

if(glowa->nastepny)
{
    element* ostracyzm_ostatniego_osobnika=glowa;
    while(glowa->nastepny->nastepny)
    {
        glowa=glowa->nastepny;
    }
    free(glowa->nastepny);
    glowa->nastepny=NULL;
    return ostracyzm_ostatniego_osobnika;
}
else
{
    free(glowa);
    return NULL;
}
}




/*printf("tez dzialam");
}
else 
{
    printf("dzialam");
}
}*/

void wypisz(element*glowa)
{
    if(glowa)
    {
        printf(" Wagon z wartoscia %d\n", glowa->liczba);
        wypisz(glowa->nastepny);
    }
    return;
}

void main()
{
    element*glowa=NULL;
    
    glowa=nowy(glowa, 1);
    glowa=nowy(glowa, 2);
    glowa=nowy(glowa, 3);
    glowa=nowy(glowa, 4);
    glowa=nowy(glowa, 7);
    glowa=nowy(glowa, -87);
    wypisz(glowa);
    printf("\nLiczba wagonow to: %d\n\n", ileichjest(glowa));
    usunostatniego(glowa);
    wypisz(glowa);
    printf("\nLiczba wagonow to: %d\n", ileichjest(glowa));
    //printf("czy dzialam\n");
    //usun(glowa);
}
//wypisz nowy ilość usunostatni usunity