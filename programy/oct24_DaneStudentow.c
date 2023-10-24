#include <stdio.h>

typedef struct 
{
    char ImieNazwisko[50];
    int waga;
    float ocena;
} dane_studenta;

void main()
{    
    dane_studenta tablica[3] = {
        { .ImieNazwisko = "Jan Kowalski", .waga = 71, .ocena = 3.5},
        { .ImieNazwisko = "Jakis ktos", .waga = 12, .ocena = 3.25},
        { .ImieNazwisko = "Nie Wiem", .waga = 191, .ocena = 33.54}  
    };

    for(int i=0; i<3; i++)
        printf("%30s | %3d | %8f\n", tablica[i].ImieNazwisko, tablica[i].waga, tablica[i].ocena);
}