#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct
{
    int wiersze, kolumny;
    float* ptr;
} macierz;

macierz utworz_macierz(int wiersze, int kolumny)
{
    return (macierz) { .wiersze = wiersze, .kolumny = kolumny, .ptr = calloc(wiersze * kolumny, sizeof(float))};
}

macierz generuj_macierz(macierz mat)
{
    srand ( time(NULL) );
    for(int i=0; i < mat.wiersze; i++)
        for(int j=0; j < mat.kolumny; j++)
            mat.ptr[i+j] = rand() % 10 + 0.01*(rand() % 100);   
    return mat;
}

void wypisz_macierz(macierz mat)
{
    for(int i=0; i < mat.wiersze; i++)
    {
        for(int j=0; j < mat.kolumny; j++)
            printf("%4.2f\t", mat.ptr[i+j]);     
        printf("\n");
    } 
    printf("\n\n");
}

macierz mnozenie(macierz mat1, macierz mat2)
{
    if(mat1.kolumny == mat2.wiersze)
    {
        macierz wynik = utworz_macierz(mat1.wiersze, mat2.kolumny);

        for (int i = 0; i < wynik.wiersze; i++)
        {
            for (int j = 0; j < wynik.kolumny; j++)
            {
                wynik.ptr[i+j] = 0;
                for (int k = 0; k < mat1.kolumny; k++)
                    wynik.ptr[i+j] += mat1.ptr[i+k] * mat2.ptr[j+k];
            }
        }
        return wynik; 
    }
    else
        printf("\nmnozenie jest niemozliwe\n");
}

void main()
{
    macierz mat1 = utworz_macierz(4,3);
    mat1 = generuj_macierz(mat1);
    wypisz_macierz(mat1);

    macierz mat2 = utworz_macierz(3,4);
    mat2 = generuj_macierz(mat2);
    wypisz_macierz(mat2);

    macierz wynik_mnozenia = mnozenie(mat1, mat2);
    wypisz_macierz(wynik_mnozenia);
}
