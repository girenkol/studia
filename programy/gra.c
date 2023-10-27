#include <stdio.h>
#include <stdlib.h>

int plansza[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

void wypisz_stan_gry()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            switch(plansza[i][j])
            {
                case 0: printf(" _ "); break;

                case 1: printf(" X "); break;
                case 2: printf(" O "); break;
            }

            if(j<2) printf("|");
        }
        printf("\n");
    }
    printf("\n");
}

void ruch_gracza(int wybor)
{
    switch(wybor)
    {
        case 1:
            if(plansza[0][0] == 0)
                plansza[0][0] = 1; break;
        case 2:
            if(plansza[0][1] == 0)
                plansza[0][1] = 1; break;
        case 3:
            if(plansza[0][2] == 0)
                plansza[0][2] = 1; break;
        case 4:
            if(plansza[1][0] == 0)
                plansza[1][0] = 1; break;
        case 5:
            if(plansza[1][1] == 0)
                plansza[1][1] = 1; break;
        case 6:
            if(plansza[1][2] == 0)
                plansza[1][2] = 1; break;
        case 7:
            if(plansza[2][0] == 0)
                plansza[2][0] = 1; break;
        case 8:
            if(plansza[2][1] == 0)
                plansza[2][1] = 1; break;
        case 9:
            if(plansza[2][2] == 0)
                plansza[2][2] = 1; break;
        default:
            printf("\n Blad!!!!!! \n\n");
    }
}

void ruch_komputera()
{
    // SprawdŸ, czy mo¿emy wygraæ w jednym ruchu
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (plansza[i][j] == 0) {
                // Spróbuj wykonac ruch, aby wygraæ
                plansza[i][j] = 2;
                if (czy_koniec() == 2) {
                    return;  // Wygraliœmy, koñcz ruch
                }
                plansza[i][j] = 0;  // Cofnij ruch
            }
        }
    }

    // Spróbuj zablokowaæ przeciwnika przed wygran¹
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (plansza[i][j] == 0) {
                // Spróbuj zablokowaæ przeciwnika
                plansza[i][j] = 1;
                if (czy_koniec() == 1) {
                    plansza[i][j] = 2;  // Wygrywamy blokuj¹c przeciwnika
                    return;
                }
                plansza[i][j] = 0;  // Cofnij ruch
            }
        }
    }

    // Spróbuj zrobiæ rozga³êzienie
    if (plansza[1][1] == 0) {
        plansza[1][1] = 2;
        return;
    }

    // Spróbuj zablokowaæ rozga³êzienie przeciwnika
    if (plansza[1][1] == 1) {
        if (plansza[0][0] == 0) {
            plansza[0][0] = 2;
            return;
        }
        if (plansza[0][2] == 0) {
            plansza[0][2] = 2;
            return;
        }
        if (plansza[2][0] == 0) {
            plansza[2][0] = 2;
            return;
        }
        if (plansza[2][2] == 0) {
            plansza[2][2] = 2;
            return;
        }
    }

    // Zagraj œrodek, jeœli jest dostêpny
    if (plansza[1][1] == 0) {
        plansza[1][1] = 2;
        return;
    }

    // Zagraj przeciwny naro¿nik, jeœli jest dostêpny
    if (plansza[0][0] == 1 && plansza[2][2] == 0) {
        plansza[2][2] = 2;
        return;
    }
    if (plansza[0][2] == 1 && plansza[2][0] == 0) {
        plansza[2][0] = 2;
        return;
    }
    if (plansza[2][0] == 1 && plansza[0][2] == 0) {
        plansza[0][2] = 2;
        return;
    }
    if (plansza[2][2] == 1 && plansza[0][0] == 0) {
        plansza[0][0] = 2;
        return;
    }

    // Zagraj pusty naro¿nik, jeœli jest dostêpny
    if (plansza[0][0] == 0) {
        plansza[0][0] = 2;
        return;
    }
    if (plansza[0][2] == 0) {
        plansza[0][2] = 2;
        return;
    }
    if (plansza[2][0] == 0) {
        plansza[2][0] = 2;
        return;
    }
    if (plansza[2][2] == 0) {
        plansza[2][2] = 2;
        return;
    }

    // Zagraj pusty bok, jeœli jest dostêpny
    for (int i = 0; i < 3; i += 2) {
        for (int j = 1; j < 3; j += 2) {
            if (plansza[i][j] == 0) {
                plansza[i][j] = 2;
                return;
            }
        }
    }
}

int czy_koniec()
{
    // Sprawdz poziome i pionowe linie
    for (int i = 0; i < 3; i++)
    {
        if (plansza[i][0] == 1 && plansza[i][1] == 1 && plansza[i][2] == 1)
            return 1; // Wygrana gracza (jedynki) w poziomie

        if (plansza[i][0] == 2 && plansza[i][1] == 2 && plansza[i][2] == 2)
            return 2; // Wygrana komputera (dwójki) w poziomie

        if (plansza[0][i] == 1 && plansza[1][i] == 1 && plansza[2][i] == 1)
            return 1; // Wygrana gracza (jedynki) w pionie

        if (plansza[0][i] == 2 && plansza[1][i] == 2 && plansza[2][i] == 2)
            return 2; // Wygrana komputera (dwójki) w pionie
    }

    // Sprawdz przekatne
    if (plansza[0][0] == 1 && plansza[1][1] == 1 && plansza[2][2] == 1)
        return 1; // Wygrana gracza (jedynki) na przekatnej

    if (plansza[0][0] == 2 && plansza[1][1] == 2 && plansza[2][2] == 2)
        return 2; // Wygrana komputera (dwójki) na przekatnej

    if (plansza[0][2] == 1 && plansza[1][1] == 1 && plansza[2][0] == 1)
        return 1; // Wygrana gracza (jedynki) na drugiej przekatnej

    if (plansza[0][2] == 2 && plansza[1][1] == 2 && plansza[2][0] == 2)
        return 2; // Wygrana komputera (dwójki) na drugiej przekatnej

    // Sprawdz remis
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (plansza[i][j] == 0)
                return 0; // Gra nie jest zakonczona (istnieja wolne pola)
        }
    }

    return -1; // Remis
}

int main()
{
    printf("\n Witaj w kolko i krzyzyk\n");
    printf("W grze wybierasz pole od 1 do 9 z klawiatury:\n 1 | 2 | 3 \n 4 | 5 | 6 \n 7 | 8 | 9 \n\n");
    wypisz_stan_gry();

    int sprawdzacz = 0;
    while(sprawdzacz == 0)
    {
        switch(czy_koniec())
        {
            case 1:
                printf("\n Wygrales!!! \n");
                sprawdzacz = 1;
                break;
            case -1:
                printf("\n Remis \n");
                sprawdzacz = 1;
                break;
            case 2:
                printf("\n Przegrales, moj algorytm cie ogral lamusie \n");
                sprawdzacz = 1;
                break;
            default:
                printf("Gdzie stawiasz X ?\n");
                int wybor;
                scanf("%d", &wybor);

                ruch_gracza(wybor);
                ruch_komputera(wybor);

                wypisz_stan_gry();
                break;
        }
    }
    for(;;);
    return 0;
}
