#include <stdio.h> 

char text[] = "pa pa nicpon pa pa nicpon"; 
char litera = 'p';
char fraza[] = "pa"; 
 
int wyszukaj_fraze(char text[], char fraza[]) 
{  
    int wystopienia = 0; 
    for(int i=0, j; text[i]; i += 1 + j) 
        for(j = 0; text[i + j] && fraza[j] && text[i+j]; !fraza[++j] && wystopienia++); 
    return wystopienia; 
} 

int dlugosc(char text[])
{
    int i=0;
    while(text[++i]);
    return i;
}

int dlugosc_bez_tablic(char *text)
{
    char *qniec = text;
    while(*qniec)
        qniec++;
    return qniec - text;
}

int wyszukaj_bez_tablic(char *text, char literka)
{
    char *wsk_text = text;
    int licznik = 0;
    while(*wsk_text){               // nie tutaj while(*wsk_text++){  to ++, ponieważ najpierw następuje zwięskzenie o 1, a dopiero sprawdzenie warunku
        if(*wsk_text++ == litera)   // tylko tutaj
            licznik++;
    }
    return licznik;
}

/*int wyszukaj_fraze_bez_tablic(char *text, char *fraza)
{
    char *wsk_text = text; 
    char *wsk_fraza = fraza;
    int licznik = 0;
    int licznik_dlugosci;

    while(*wsk_text){
        licznik_dlugosci = 0;
        while(*wsk_fraza == *wsk_text && *wsk_fraza && *wsk_text){
            wsk_fraza++;
            wsk_text++;
            licznik_dlugosci++;
        }
        if(licznik_dlugosci == dlugosc_bez_tablic(fraza))
            licznik++;
        wsk_text++;
    }
    return licznik;
}*/

int wyszukaj_fraze_bez_tablic(char *text, char *fraza)
{
    char *wsk_text = text;
    char *wsk_fraza = fraza;
    int licznik = 0;
    for(int i=0, j; *wsk_text; *wsk_text += *wsk_fraza + 1)
        for(j=0; (*wsk_text += *wsk_fraza) && *wsk_fraza && ((*wsk_text += *wsk_fraza) == *wsk_fraza) && j++; licznik++);
    return licznik;
}

void main()
{ 
    //printf("w text jest %d frazy \n", wyszukaj_fraze(text, "plusy")); 

    printf("dlugosc textu: %s wynosi: %d\n", text, dlugosc_bez_tablic(text));

    printf("w textcie: %s wystepuje razy: %d\n", text, wyszukaj_bez_tablic(text, litera));

    printf("w textcie: %s fraza wystepuje razy: %d\n", text, wyszukaj_fraze_bez_tablic(text, fraza));
}