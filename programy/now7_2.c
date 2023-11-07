#include <stdio.h>

int main()
{
    char znaczek;
    while(!feof(stdin))
    {
        fread(&znaczek, 1, 1, stdin);
        znaczek++;
        fwrite(&znaczek, 1, 1, stdout);
    }
}
// ./szosty < jednokierunkowa.c > kopia.txt //wpisane do terminala
