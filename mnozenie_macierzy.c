#include <stdio.h> 

/*void mnozenie(float lewa[], float prawa[], float wynik[], int rozmiar) 
{ 
    for(int wiersz = 0; wiersz < rozmiar; wiersz++) 
        for(int) 
}*/ 

/*
int mnozenie(float lewa[], float prawa[], float wynik[]) 
{ 
    int wynik_p; 
    for(int i=0, j=0, k = 0; i < 3; i++){ 
        for(j=0; j < 3; j++){ 
            wynik[indeksy2indeks(i,j)] = 0; 
            for(k=0; k < 3; k++){ 
                wynik[indeksy2indeks(i,j)] += lewa[j,k] + prawa[k,j]; 
            } 
        } 
        wynik[indeksy2indeks(i,j)] = wynik_p; 
    } 
    return wynik; 
} */


int indeksy2indeks(int wiersz, int kolumna) 
{ 
    return kolumna + wiersz*3;  
} 

int mnozenie(float lewa[], float prawa[], float wynik[]) 
{ 
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            wynik[indeksy2indeks(i, j)] = 0;
            for (int k = 0; k < 3; k++) { 
                wynik[indeksy2indeks(i, j)] += lewa[i * 3 + k] * prawa[k * 3 + j]; 
            }
        }
    }
}

void main() 
{ 
    float lewa[9] = {1,0,2, 
                     -1,3,-1, 
                     0,2,2},  

        prawa[9] = {1,2,3, 
                    -1,2,0, 
                    0,3,-4},  
        wynik[9]; 
    /* 

        powinno wyjść  

        1 8 -5 

        -4 1 1 

        -2 10 -8 

    */ 

    mnozenie(lewa, prawa, wynik); 

    for(int i=0; i < 9; i++){ 
        printf("%f ", wynik[i]); 
        if(i %3 == 2) 
            printf("\n"); 
    } 
} 
//zrobic wyznacznik odwrotnosc mnozenie macierzy
