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
            mat.ptr[i * mat.kolumny + j] = rand() % 20 + 0.01*(rand() % 100) - 10;

    return mat;
}

void wypisz_macierz(macierz mat)
{
    for(int i=0; i < mat.wiersze; i++)
    {
        for(int j=0; j < mat.kolumny; j++)
        {
            if(mat.ptr[i*mat.kolumny+j] >= 0) // zeby ujemne nie przeszkadzaly wizualnie
                printf(" ");
            printf("%4.2f\t", mat.ptr[i*mat.kolumny+j]);
        }

        printf("\n");
    }
    printf("\n\n");
}

macierz mnozenie(macierz mat1, macierz mat2)
{
    if (mat1.kolumny == mat2.wiersze) // warunek mnozenia macierzy
    {
        macierz wynik = utworz_macierz(mat1.wiersze, mat2.kolumny); // tworzy macierz do wpisywania wyniku

        for (int i = 0; i < wynik.wiersze; i++)
        {
            for (int j = 0; j < wynik.kolumny; j++) // te fory leca po wszyskich elementach macierzy
            {
                wynik.ptr[i * wynik.kolumny + j] = 0; // zeruje wszystko
                for (int k = 0; k < mat1.kolumny; k++) // leci po kolumnach mat1 i wierszach mat2 dodaje to wszystko do komorki w wynik
                    wynik.ptr[i * wynik.kolumny + j] += mat1.ptr[i * mat1.kolumny + k] * mat2.ptr[k * mat2.kolumny + j];
            }
        }
        return wynik;
    }
    else
    {
        printf("\nmnozenie jest niemozliwe\n");
        exit(1); // konczy funkcje
    }
}

float det(macierz mat)
{
    if(mat.wiersze != mat.kolumny) // jak nie jest kwadratowa to nie ma wyznacznika
    {
        printf("\n kolumny != wiersze, wiec sie nie da\n");
        return 0; // konczy funkcje
    }

    if(mat.wiersze == 2) // dla 2x2 nie wchodzi w bardziej skompikowane obliczenia, zwraca wynik i przerywa funkcje
    {
        return ((mat.ptr[0] * mat.ptr[3]) - (mat.ptr[1] * mat.ptr[2]));
    }

    float wynik = 0;
    int czynnik = 1;

    for(int i = 0; i < mat.wiersze; i++)
    {
        macierz podmat = utworz_macierz(mat.wiersze - 1, mat.kolumny - 1); // tworzy podmacierz o skreślonej kolumnie mniejszą

        for (int wier = 0; wier < mat.wiersze; wier++) //wier = 1
        {
            for (int kol = 0, kolPodmat = 0; kol < mat.kolumny; kol++) // skacze po wszystkich elementach mat mat
            {
                if (kol != i) // pomija to co wykreslone
                {
                    podmat.ptr[(wier - 1) * (mat.kolumny - 1) + kolPodmat] = mat.ptr[wier * mat.kolumny + kol]; //wypelnienie podmacierzy elementami z macierzy mat odpowiednim dopelnieniem
                    kolPodmat++;
                }
            }
        }

        wynik += czynnik * mat.ptr[i] * det(podmat); //obliczenia zgodnie ze wzorem i przypisanie danych w mat wynik
        czynnik = -czynnik; // zmienia znak zgodnie z (-1)^mn
    }

    return wynik;
}

macierz odwrotna(macierz mat)
{
    if (det(mat) == 0) // warunek czy macierz moze byc odwracana
    {
        printf("det(mat) = 0 => odwrotność niemożliwa\n");
        exit(1); //przerywa funkcje
    }

    float detMat = det(mat);
    macierz wynik = utworz_macierz(mat.wiersze, mat.kolumny); // wpisanie do detMat wyznacznika tej mat i init mat wynik takiego samego rozm

    int czynnik = 1;
    for (int i = 0; i < mat.wiersze; i++)
    {
        for (int j = 0; j < mat.kolumny; j++) // skacze po wszystkich elementach mat mat
        {

            macierz podmat = utworz_macierz(mat.wiersze - 1, mat.kolumny - 1); // macierz podmat, ktora jest macierza bez skreslonego i-tego wiersza i j-tej kolumny, tworzy dla kdazdego elementu macierz dopelniajaca

            int k = 0;
            for (int m = 0; m < mat.wiersze; m++)
            {
                if (m != i) //for leci po elementach macierzy ale pomija elementy wiersza skreslonego a nizej kolumny
                {
                    int l = 0;

                    for (int n = 0; n < mat.kolumny; n++)
                    {
                        if (n != j)
                        {
                            podmat.ptr[k * podmat.kolumny + l] = mat.ptr[m * mat.kolumny + n]; //wypelnienie podmacierzy elementami z macierzy mat odpowiednim dopelnieniem
                            l++;
                        }
                    }

                    k++;
                }
            }

            // float wsp = det(podmat) / detMat; // wynik.ptr[j * wynik.kolumny + i] = czynnik * wsk;

            wynik.ptr[j * wynik.kolumny + i] = czynnik * det(podmat) / detMat;  //obliczenia zgodnie ze wzorem i przypisanie danych w mat wynik, z transponowaniem macierzy: zamiana indeksow i i j

            czynnik = -czynnik; //odwraca znak zgodnie z (-1)^mn
        }
    }

    return wynik;
}

void main()
{
    printf("> mat1\n");
    macierz mat1 = utworz_macierz(4,4);
    mat1 = generuj_macierz(mat1);
    wypisz_macierz(mat1);

    float wynik = det(mat1);
    printf("Wyznacznik macierzy mat1: %.2f\n\n", wynik);

    printf("> mat2\n");
    macierz mat2 = utworz_macierz(4,3);
    mat2 = generuj_macierz(mat2);
    wypisz_macierz(mat2);

    printf("> mat1 x mat2\n");
    macierz wynik_mnozenia = mnozenie(mat1, mat2);
    wypisz_macierz(wynik_mnozenia);

    printf("> (mat1)^-1\n");
    macierz wynik_odwrotnosci = odwrotna(mat1);
    wypisz_macierz(wynik_odwrotnosci);


}
