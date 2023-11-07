#include <stdio.h>

/*int main()
{
    char bufor[1024];
    FILE *plik = fopen("nov7.txt", "r");
    while(!feof(plik))
    {
        fscanf(plik, "%[^\n]\n", bufor);
        //fscanf(plik, "\n");
        fseek(plik, 0, SEEK_SET);
        printf("%s\n", bufor);
    }
    fclose(plik);
}*/

/*int main()
{
    float liczby[] = {0, 1, 2, 3, 4, 5 ,6, 7, 8 ,9};

    FILE *plik = fopen("liczby", "W+");

    //fwrite(liczby, sizeof(float), 10, plik); //tu nie tworzy pliku nw czemu

    //fread(liczby, sizeof(float), 10, plik);

    fclose(plik);
}*/

/*
int main() {
    // Definiowanie rozmiaru macierzy
    int rows = 3;
    int cols = 3;

    // Definiowanie macierzy A i B
    int matrixA[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int matrixB[3][3] = {{2, 3, 4}, {1, 2, 1}, {5, 6, 5}};

    // Obliczanie ilorazu macierzy i zapisywanie wyniku do macierzy C
    int matrixC[3][3];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrixC[i][j] = matrixA[i][j] / matrixB[i][j];
        }
    }

    // Nazwa pliku, do którego zostan¹ zapisane macierze
    const char *filename = "macierze.txt";

    // Otwarcie pliku w trybie zapisu
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Nie mo¿na otworzyæ pliku do zapisu.\n");
        return 1;
    }

    // Zapisywanie macierzy A do pliku
    fprintf(file, "Macierz A:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrixA[i][j]);
        }
        fprintf(file, "\n");
    }

    // Zapisywanie macierzy B do pliku
    fprintf(file, "Macierz B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrixB[i][j]);
        }
        fprintf(file, "\n");
    }

    // Zapisywanie ilorazu macierzy C do pliku
    fprintf(file, "Iloraz macierzy A i B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrixC[i][j]);
        }
        fprintf(file, "\n");
    }

    // Zamkniêcie pliku
    fclose(file);

    printf("Macierze oraz ich iloraz zosta³y zapisane do pliku \"%s\".\n", filename);

    return 0;
}*/



int main() {
    // Definiowanie rozmiaru macierzy
    int rows, cols;

    printf("Podaj liczbe wierszy macierzy: ");
    scanf("%d", &rows);
    printf("Podaj liczbe kolumn macierzy: ");
    scanf("%d", &cols);

    // Definiowanie macierzy A i B
    int matrixA[rows][cols];
    int matrixB[rows][cols];

    // Wprowadzanie wartości macierzy A
    printf("Wprowadz wartosci macierzy A:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Wprowadz element [%d][%d]: ", i, j);
            scanf("%d", &matrixA[i][j]);
        }
    }

    // Wprowadzanie wartości macierzy B
    printf("Wprowadz wartosci macierzy B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Wprowadz element [%d][%d]: ", i, j);
            scanf("%d", &matrixB[i][j]);
        }
    }

    // Obliczanie ilorazu macierzy i zapisywanie wyniku do macierzy C
    int matrixC[rows][cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrixC[i][j] = matrixA[i][j] / matrixB[i][j];
        }
    }

    // Nazwa pliku, do którego zostaną zapisane macierze
    const char *filename = "macierze.txt";

    // Otwarcie pliku w trybie zapisu
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Nie można otworzyć pliku do zapisu.\n");
        return 1;
    }

    // Zapisywanie macierzy A do pliku
    fprintf(file, "Macierz A:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrixA[i][j]);
        }
        fprintf(file, "\n");
    }

    // Zapisywanie macierzy B do pliku
    fprintf(file, "Macierz B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrixB[i][j]);
        }
        fprintf(file, "\n");
    }

    // Zapisywanie ilorazu macierzy C do pliku
    fprintf(file, "Iloraz macierzy A i B:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrixC[i][j]);
        }
        fprintf(file, "\n");
    }

    // Zamknięcie pliku
    fclose(file);

    printf("Macierze oraz ich iloraz zostały zapisane do pliku \"%s\".\n", filename);

    return 0;
}


