**INSTRUKCJA LABORATORIUM: KALIBRACJA, HOMOGRAFIA I LOKALIZACJA 3D**

**1. ETAP PRZYGOTOWAWCZY (MATLAB)**

* **Pozyskanie obrazów:** Wykonaj ok. 50 zdjęć szachownicy w rozdzielczości docelowej (np. 1920x1080). Szachownica musi znaleźć się w każdym narożniku i przy każdej krawędzi kadru.
* **Aplikacja Camera Calibrator:**
    - Wczytaj zdjęcia i podaj fizyczny wymiar boku kwadratu (np. 25 mm).
    - W opcjach zaznacz: 2 Radial Coefficients oraz Tangential Distortion.
    - Po kalibracji usuń zdjęcia z błędem reprojekcji > 0.5 px i powtórz obliczenia.
* **Eksport danych:** Kliknij Export Parameters i zaznacz opcję Include Estimation Errors.
* **Wizualizacja zniekształceń:**
    - Otwórz skrypt visualize_distortions2.m.
    - Zmień wartości nx i ny na rozdzielczość swoich zdjęć (np. nx=1920, ny=1080).
    - Uruchom skrypt, aby wygenerować wykresy błędu soczewki.
* **Weryfikacja:** Użyj funkcji undistort na pierwszym zdjęciu i sprawdź, czy krawędzie szachownicy stały się idealnie proste.

**2. KONFIGURACJA ŚRODOWISKA (VISUAL STUDIO)**

* **Uruchomienie projektu:** Otwórz plik wizja_2024_stud.sln.
* **Tryb pracy:** Na górnym pasku ustaw tryb Release oraz architekturę x64.
* **Uzupełnienie parametrów w main_2024.cpp:**
    - Wpisz wartości fc (FocalLength) i cc (PrincipalPoint) do macierzy macierzKamery.
    - Wpisz współczynniki kc (Radial i Tangential) do macierzy wspolczynnikiZnieksztalcen w kolejności: [k1, k2, p1, p2, k3].
* **Skalowanie (jeśli rozdzielczość pracy jest inna niż zdjęć):**
    - fc_new = fc_old * (Szerokosc_pracy / Szerokosc_zdjec)
    - cc_new = cc_old * (Szerokosc_pracy / Szerokosc_zdjec)

**3. REALIZACJA ZADAŃ W KODZIE C++**

**Zadanie: Homografia (Prostowanie obrazu)**
1. Odkomentuj homografia_z_kartka() w funkcji main.
2. Uruchom program i kliknij 4 narożniki kartki na obrazie.
3. W kodzie zdefiniuj 4 punkty fizyczne (np. (0,0), (210,0), (210,297), (0,297) dla kartki A4).
4. Kolejność klikania musi być identyczna z kolejnością punktów w kodzie.
5. Aby przesunąć widok i zobaczyć otoczenie, ustaw pierwszy punkt na np. (100, 100).

**Zadanie: Lokalizacja 3D i Szachownica**
1. W pliku szachownica.h sprawdź definicję klasy: Szachownica(liczba_punktow_X, liczba_punktow_Y, wymiar_mm).
2. Pamiętaj: jeśli szachownica ma 10 pól, podajesz 9 punktów styku.
3. Odkomentuj lokalizacja_3D() w main. Program powinien narysować kolorowe osie:
    - Czerwona (X) - pozioma.
    - Zielona (Y) - pionowa.
    - Niebieska (Z) - oś skierowana "w głąb" lub "od" szachownicy.
4. Uruchom lokalizacja_szachownic(), aby wyznaczyć relację między dwoma wzorcami.
5. **Weryfikacja:** Zmierz fizyczną odległość między początkami układów współrzędnych obu szachownic i porównaj z wynikiem na ekranie.

**4. TYPOWE PROBLEMY I ROZWIĄZANIA**

* **Brak osi 3D:** Sprawdź, czy macierz kamery nie zawiera samych zer i czy rozdzielczość zdjęć pasuje do parametrów.
* **Obraz "skacze" (laguje):** Upewnij się, że pracujesz w trybie Release.
* **Szachownica niewykrywana:** Popraw oświetlenie lub zmniejsz rozdzielczość w kodzie (pamiętając o skalowaniu fc i cc)."""
