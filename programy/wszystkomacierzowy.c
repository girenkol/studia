#include <stdio.h>

typedef struct {
    int wiersze;
    int kolumny;
    double dane[100][100];
} Macierz;


void wypisz(Macierz mat)
{
    for (int i = 0; i < mat.wiersze; i++)
    {
        for (int j = 0; j < mat.kolumny; j++)
            printf("%.1lf\t", mat.dane[i][j]);

        printf("\n");
    }
}

Macierz mnozenie(Macierz mat1, Macierz mat2)
{
    Macierz wynik;

    if (mat1.kolumny != mat2.wiersze)
    {
        printf("Nie mozna pomnozyc macierzy - nieodpowiednie rozmiary.\n");
        wynik.wiersze = 0;
        wynik.kolumny = 0;
        return wynik;
    }

    wynik.wiersze = mat1.wiersze;
    wynik.kolumny = mat2.kolumny;

    for (int i = 0; i < wynik.wiersze; i++)
        {
        for (int j = 0; j < wynik.kolumny; j++)
        {
            wynik.dane[i][j] = 0;
            for (int k = 0; k < mat1.kolumny; k++)
                wynik.dane[i][j] += mat1.dane[i][k] * mat2.dane[k][j];
        }
    }

    return wynik;
}

double wyznacznik(Macierz mat) {
    if (mat.wiersze != mat.kolumny) {
        printf("Nie można obliczyć wyznacznika - macierz nie jest kwadratowa.\n");
        return 0.0;
    }

    if (mat.wiersze == 1)
        return mat.dane[0][0];

    double det = 0;
    int znak = 1;

    for (int k = 0; k < mat.wiersze; k++)
    {
        Macierz podmacierz;
        podmacierz.wiersze = mat.wiersze - 1;
        podmacierz.kolumny = mat.kolumny - 1;

        for (int i = 1; i < mat.wiersze; i++)
        {
            int j2 = 0;
            for (int j = 0; j < mat.kolumny; j++)
                if (j != k)
                {
                    podmacierz.dane[i - 1][j2] = mat.dane[i][j];
                    j2++;
                }
        }

        det += znak * mat.dane[0][k] * wyznacznik(podmacierz);
        znak = -znak;
    }

    return det;
}

Macierz odwroc(Macierz mat)
{
    if (mat.wiersze != mat.kolumny)
    {
        printf("\nNie mozna odwrocic macierzy bo nie jest kwadratowa\n");
        Macierz pusta;
        pusta.wiersze = 0;
        pusta.kolumny = 0;
        return pusta;
    }

    int n = mat.wiersze;
    Macierz rozszerzona;
    rozszerzona.wiersze = n;
    rozszerzona.kolumny = 2 * n;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            rozszerzona.dane[i][j] = mat.dane[i][j];

            if (i == j)
                rozszerzona.dane[i][n + j] = 1.0;
            else
                rozszerzona.dane[i][n + j] = 0.0;
        }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
            {
                double wspolczynnik = rozszerzona.dane[j][i] / rozszerzona.dane[i][i];

                for (int k = 0; k < 2 * n; k++)
                    rozszerzona.dane[j][k] -= wspolczynnik * rozszerzona.dane[i][k];
            }

    for (int i = 0; i < n; i++)
    {
        double dzielnik = rozszerzona.dane[i][i];
        for (int j = 0; j < 2 * n; j++)
            rozszerzona.dane[i][j] /= dzielnik;
    }

    Macierz odwrocona;
    odwrocona.wiersze = n;
    odwrocona.kolumny = n;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            odwrocona.dane[i][j] = rozszerzona.dane[i][n + j];

    return odwrocona;
}

int main()
{
    Macierz mat1 =
    {
        .wiersze = 3,
        .kolumny = 3,
        .dane =
        {
            {1.0, 6.0, 3.0},
            {4.0, 5.2, 6.9},
            {7.0, 8.1, 9.0}
        }
    };

    Macierz mat2 =
    {
        .wiersze = 3,
        .kolumny = 2,
        .dane =
        {
            {2.0, 1.4},
            {1.3, 2.0},
            {3.0, 4.0}
        }
    };

    printf("mat 1\n");
    wypisz(mat1);

    printf("\nWyznacznik mat: %f\n", wyznacznik(mat1));

    printf("\nmat 2\n");
    wypisz(mat2);

    Macierz wynik = mnozenie(mat1, mat2);

    printf("\nmat1 x mat2\n");
    wypisz(wynik);

    Macierz odwrocona = odwroc(mat1);
    odwroc(odwrocona);
    printf("\nmacierz odwrotna do mat1:\n");
    wypisz(odwrocona);

    return 0;
}
