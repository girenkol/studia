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
            mat.ptr[i * mat.kolumny + j] = rand() % 10 + 0.01*(rand() % 100);

    return mat;
}

void wypisz_macierz(macierz mat)
{
    for(int i=0; i < mat.wiersze; i++)
    {
        for(int j=0; j < mat.kolumny; j++)
            printf("%4.2f\t", mat.ptr[i*mat.kolumny+j]);
        printf("\n");
    }
    printf("\n\n");
}

macierz mnozenie(macierz mat1, macierz mat2)
{
    if (mat1.kolumny == mat2.wiersze)
    {
        macierz wynik = utworz_macierz(mat1.wiersze, mat2.kolumny);

        for (int i = 0; i < wynik.wiersze; i++)
        {
            for (int j = 0; j < wynik.kolumny; j++)
            {
                wynik.ptr[i * wynik.kolumny + j] = 0;
                for (int k = 0; k < mat1.kolumny; k++)
                    wynik.ptr[i * wynik.kolumny + j] += mat1.ptr[i * mat1.kolumny + k] * mat2.ptr[k * mat2.kolumny + j];
            }
        }
        return wynik;
    }
    else
    {
        printf("\nmnozenie jest niemozliwe\n");
        return utworz_macierz(0, 0);
    }
}

float det(macierz mat)
{
    if(mat.wiersze != mat.kolumny)
    {
        printf("\n kolumny != wiersze, wiec sie nie da\n");
        return 0;
    }

    if(mat.wiersze == 2)
    {
        return ((mat.ptr[0] * mat.ptr[3]) - (mat.ptr[1] * mat.ptr[2]));
    }

    float wynik = 0;
    int czynnik = 1;

    for(int i = 0; i < mat.wiersze; i++)
    {
        macierz podmat = utworz_macierz(mat.wiersze - 1, mat.kolumny - 1); // tworzy podmacierz o skreślonej kolumnie mniejszą

        for (int wier = 1; wier < mat.wiersze; wier++)
        {
            for (int kol = 0, kolPodmat = 0; kol < mat.kolumny; kol++)
            {
                if (kol != i)
                {
                    podmat.ptr[(wier - 1) * (mat.kolumny - 1) + kolPodmat] = mat.ptr[wier * mat.kolumny + kol];
                    kolPodmat++;
                }
            }
        }

        wynik += czynnik * mat.ptr[i] * det(podmat);
        czynnik = -czynnik;
    }

    return wynik;
}

macierz odwrotna(macierz mat)
{
    if(det(mat) == 0) // czy sie da odw
    {
        printf("det(mat)=0 => odwrotnosc niemozliwa\n");
        return;
    }
    
    float detMat = det(mat);
    macierz wynik = utworz_macierz(mat.wiersze, mat.kolumny);

    int czynnik = 1;
    /*
    for(int wier = mat.wiersze, i=0; wier > 0; wier--, i++) // to ma byc transpionowana, robie filpa i i to jest kolumna a nie wiersz no i j to wiersz
    {
        for(int kol = mat.kolumny, j=0; kol > 0; kol--, j++)
        {
            //wynik.ptr[i * mat.kolumny + j] = (czynnik * mat.ptr[wier * mat.kolumny + kol])/detMat;

            czynnik = -czynnik;
        }
    }
    */
    for(int i = -1, wier = mat.wiersze; i <= mat.wiersze; i++, wier--) 
    {
        for(int j = -1, kol = mat.kolumny; j <= mat.kolumny; j++, kol--)
        {
            wynik.ptr[i * wynik.kolumny + j] =  ( czynnik * mat.ptr[wier * mat.kolumny + kol] ) / detMat;
            czynnik = -czynnik;
        }
    }

    return wynik;
}

void main()
{
    printf("> mat1\n");
    macierz mat1 = utworz_macierz(3,3);
    mat1 = generuj_macierz(mat1);
    wypisz_macierz(mat1);

    printf("> mat2\n");
    macierz mat2 = utworz_macierz(3,2);
    mat2 = generuj_macierz(mat2);
    wypisz_macierz(mat2);

    printf("> mat1 x mat2\n");
    macierz wynik_mnozenia = mnozenie(mat1, mat2);
    wypisz_macierz(wynik_mnozenia);

    printf("> (mat1)^-1\n");
    macierz wynik_odwrotnosci = odwrotna(mat1);
    wypisz_macierz(wynik_odwrotnosci);

    float wynik = det(mat1);
    printf("Wyznacznik macierzy mat1: %.2f\n", wynik);
}
