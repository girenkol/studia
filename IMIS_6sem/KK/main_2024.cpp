#include "pomocnicze.h"

// inst: Tutaj wpisz parametry z Matlaba (fc i cc). 
// inst: Jeśli zdjęcia do kalibracji miały inną rozdzielczość niż ta, na której pracujesz teraz, 
// inst: pomnóż fc oraz cc przez współczynnik: (obecna_szerokosc / szerokosc_zdjec_z_kalibracji).
// inst: Format: fx, 0, cx, 0, fy, cy, 0, 0, 1
Mat macierzKamery = (Mat_<double>(3, 3) << 
    0.0, 0,   0.0,  // fx, 0, cx
    0,   0.0, 0.0,  // 0, fy, cy
    0,   0,   1);

// inst: Wpisz współczynniki kc w kolejności: [k1, k2, p1, p2, k3]. 
// inst: Jeśli w Matlabie miałeś tylko 2 parametry radialne, wpisz je jako k1 i k2, a resztę ustaw na 0.
Mat wspolczynnikiZnieksztalcen = (Mat_<double>(1, 5) << 0.0, 0.0, 0.0, 0.0, 0.0);

int main()
{
    // --- KROK 1: HOMOGRAFIA ---
    // inst: Odkomentuj poniższą linię, aby uruchomić zadanie z homografią (prostowanie obrazu kartki).
    // inst: Po uruchomieniu kliknij 4 narożniki kartki w tej samej kolejności, w jakiej podasz współrzędne w milimetrach.
    // homografia_z_kartka();

    // --- KROK 2: LOKALIZACJA 3D (POJEDYNCZA SZACHOWNICA) ---
    // inst: Odkomentuj poniższą linię, aby wyznaczyć pozycję szachownicy w przestrzeni względem kamery.
    // inst: Pamiętaj, aby w pliku lokalizacja_szachownic.h (lub wewnątrz funkcji) 
    // inst: poprawnie zdefiniować obiekt: Szachownica szachownica(liczba_rozkow_X, liczba_rozkow_Y, rozmiar_bok_mm);
    // lokalizacja_3D();

    // --- KROK 3: RELACJA MIĘDZY DWOMA SZACHOWNICAMI ---
    // inst: Ta funkcja służy do wyznaczania odległości między dwoma wzorcami (zadanie z linijką).
    // lokalizacja_szachownic();

    // inst: Standardowo program uruchamia funkcję KEN (prawdopodobnie demonstracyjną). 
    // inst: Zakomentuj ją, gdy będziesz wykonywać zadania z instrukcji.
    KEN();
    
    return 0;
}
