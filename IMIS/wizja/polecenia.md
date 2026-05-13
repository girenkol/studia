**Zadania do zrobienia (Kamera w ruchu drogowym)**

1. Wyznaczyć homografię H1 na podstawie pasów (zebry) by widzieć obszar od maski samochodu do równo 50 m za pasami, a na szerokość nieco więcej niż w sumie trzy pasy ruchu na jezdni (czyli nasz pas oraz pas po lewej i po prawej).
2. Wyznaczyć położenie pasów względem kamery i samochodu względem pasów.
3. Narysować wersory osi.
4. Wyznaczyć homografię H2, która przelicza współrzędne w metrach względem pasów na współrzędne pikselowe.
5. Wykorzystać tę homografię (H2) do narysowania 5 linii na ulicy w widoku perspektywicznym, co 10 metrów, począwszy od dalszej krawędzi pasów.
6. Wykorzystać połączenie obu homografii do narysowania takich samych linii na obrazie wyprostowanym.
7. Narysować bryły w miejscu wszystkich pasów na jezdni. Co stanie się z bryłą, gdy zamiast punkty_na_pasach 3D użyjemy punkty_na_pasach2D?
8. Załadować sieć Yolo i wykryć samochody i pieszych.
9. Sprawdzić jak na skuteczność detekcji i prędkość działania programu wpływa zmiana parametrów sieci i samej sieci.
10. Uruchomić wszystko w pętli przetwarzającej cały film.
11. Wykorzystać punkty zaczepienia pojazdów i wykorzystując odpowiednie homografie wyznaczyć odległość pojazdu od samochodu.
12. Narysować prostokąt na obrazie wyprostowanym w miejscu, w którym znajduje się samochód.
13. Narysować trójwymiarową bryłę w miejscu, w którym znajduje się samochód.
14. Powtórzyć punkty 12 i 13 dla pieszych, odpowiednio modyfikując geometrię bryły.
15. Wykorzystać zdobytą wiedzę i napisany kod do uzyskania podobnego efektu na ujęciu z al. KEN zgodnie z opisem w sekcji 6.
