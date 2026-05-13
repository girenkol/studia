#pragma once

#include "opencv2\opencv.hpp"
#include <filesystem>

using namespace cv;
using namespace std;

extern vector<Point2f> punkty_na_obrazie;
extern vector<Point2f> H_punkty_klikniete;
extern vector<Point2f> H_punkty_przeliczone;
extern  Mat obraz_z_kamery;

int ulica()
{
    /* # Wczytanie wideo i ustawienie klatki poczatkowej */
    VideoCapture kamera("ulice4.mp4");
    if (!kamera.isOpened()) {
        cout << "\n\nNie mozna zaladowac pliku z filmem!\nNacisnij ENTER by zakonczyc program!";
        cin.get();
        return 0;
    }
    
    kamera.set(CAP_PROP_POS_FRAMES, 4000);
    kamera >> obraz_z_kamery;
    Size imageSize = obraz_z_kamery.size();

    /* # Przygotowanie map do usuniecia znieksztalcen soczewkowych */
    Mat map1, map2, obraz_oczyszczony;
    cv::initUndistortRectifyMap(macierzKamery, wspolczynnikiZnieksztalcen, Mat(), macierzKamery, imageSize, CV_16SC2, map1, map2);
    
    /* # Nalezy usunac znieksztalcenia z pierwszej klatki przed wyznaczeniem punktow! */
    cv::remap(obraz_z_kamery, obraz_oczyszczony, map1, map2, INTER_LINEAR);
    obraz_z_kamery = obraz_oczyszczony.clone();

    /* # ZADANIE 1: Homografia H1 - widok z lotu ptaka */
    punkty_na_obrazie = { Point2f(450, 763), Point2f(980, 773), Point2f(1148, 866), Point2f(352, 859) };
    
    float s = 0.15;
    int dx = 500;
    int dy = 5000;
    vector<Point2f> punkty_na_pasach2D = { {dx * s, dy * s}, { (dx + 350) * s,dy * s}, { (dx + 350) * s,(dy + 400) * s}, { dx * s,(dy + 400) * s } };
    Mat H1 = findHomography(punkty_na_obrazie, punkty_na_pasach2D);
    
    Mat wyprostowany;

    /* # ZADANIE 2: Lokalizacja pasow wzgledem kamery */
    vector<Point3f> punkty_na_pasach3D = { {0,0,0}, {350,0,0}, {350,-400,0}, {0,-400,0} };
    Mat trans, rot;
    solvePnP(punkty_na_pasach3D, punkty_na_obrazie, macierzKamery, wspolczynnikiZnieksztalcen, rot, trans);
    
    Mat T_ko = Mat::eye(4, 4, CV_64F);
    Rodrigues(rot, T_ko(Rect(0, 0, 3, 3)));
    Mat(trans).copyTo(T_ko(Rect(3, 0, 1, 3)));

    /* # ZADANIE 3: Wersory osi przygotowane */
    vector<Point3f> wersory3D = { {0,0,0}, {100,0,0}, {0,100,0}, {0,0,100} };
    vector<Point2f> wersory2D;

    /* # ZADANIE 4: Homografia H2 (wspolrzedne samochodowe -> pikselowe) */
    vector<Point2f> punkty_samochodowe2D = { {-1.3, 10.0}, { 2.2, 10.0}, { 2.2, 6.0}, { -1.3, 6.0 } };
    Mat H2 = findHomography(punkty_samochodowe2D, punkty_na_obrazie);

    /* # H3: z metrow (uklad samochodu) na centymetry (uklad pasow) do generacji bryl 3D */
    vector<Point2f> punkty_na_pasach_xy = { {0,0}, {350,0}, {350,-400}, {0,-400} };
    Mat H3 = findHomography(punkty_samochodowe2D, punkty_na_pasach_xy);

    /* # ZADANIE 8: Inicjalizacja sieci YOLO poza petla */
    Yolo yolo("yolov4-tiny.cfg", "yolov4-tiny.weights", "classes.txt", 0.4, 0.2);

    /* # ZADANIE 10: Glowna petla przetwarzajaca film */
    while (waitKey(1) != 27)
    {
        kamera >> obraz_z_kamery;
        if (obraz_z_kamery.empty()) break;

        /* # Bezwzglednie usun znieksztalcenia soczewkowe przed jakimikolwiek operacjami */
        cv::remap(obraz_z_kamery, obraz_oczyszczony, map1, map2, INTER_LINEAR);
        obraz_z_kamery = obraz_oczyszczony.clone();

        /* # Przeksztalcenie na widok z lotu ptaka */
        warpPerspective(obraz_z_kamery, wyprostowany, H1, Size((350 + dx + 220) * s, (400 + dy + 300) * s));

        /* # Rysowanie wersorow (Zadanie 3) */
        projectPoints(wersory3D, rot, trans, macierzKamery, wspolczynnikiZnieksztalcen, wersory2D);
        line(obraz_z_kamery, wersory2D[0], wersory2D[1], Scalar(0, 0, 255), 2);
        line(obraz_z_kamery, wersory2D[0], wersory2D[2], Scalar(0, 255, 0), 2);
        line(obraz_z_kamery, wersory2D[0], wersory2D[3], Scalar(255, 0, 0), 2);

        /* # ZADANIE 5 i 6: Rysowanie 5 linii na jezdni co 10m od krawedzi pasow */
        for (int i = 1; i <= 5; ++i) {
            float dist = i * 10.0f;
            line(obraz_z_kamery, transformuj_wspolrzedne(Point2f(-3.0, dist), H2), transformuj_wspolrzedne(Point2f(3.0, dist), H2), Scalar(0, 255, 255), 2);
            line(wyprostowany, transformuj_wspolrzedne(Point2f(-3.0, dist), H1 * H2), transformuj_wspolrzedne(Point2f(3.0, dist), H1 * H2), Scalar(0, 255, 255), 2);
        }

        /* # ZADANIE 7: Narysowac bryly w miejscu wszystkich pasow (4 sztuki) */
        for (int i = 0; i < 4; i++) {
            float start_x = i * 100.0f; /* # Szerokosc pasa 50cm, przerwa 50cm */
            vector<Point3f> p_dolna3D = { {start_x, 0, 0}, {start_x + 50, 0, 0}, {start_x + 50, -400, 0}, {start_x, -400, 0} };
            vector<Point3f> p_gorna3D;
            
            for (auto p : p_dolna3D) {
                p_gorna3D.push_back(Point3f(p.x, p.y, -30)); /* # Wysokosc bryly 30cm (dla czytelnosci) */
            }

            vector<Point2f> p_dolna2D, p_gorna2D;
            projectPoints(p_dolna3D, rot, trans, macierzKamery, wspolczynnikiZnieksztalcen, p_dolna2D);
            projectPoints(p_gorna3D, rot, trans, macierzKamery, wspolczynnikiZnieksztalcen, p_gorna2D);

            for (int j = 0; j < 4; j++) {
                line(obraz_z_kamery, p_dolna2D[j], p_dolna2D[(j + 1) % 4], Scalar(255, 100, 0), 2);
                line(obraz_z_kamery, p_gorna2D[j], p_gorna2D[(j + 1) % 4], Scalar(255, 100, 0), 2);
                line(obraz_z_kamery, p_dolna2D[j], p_gorna2D[j], Scalar(255, 100, 0), 2);
            }
        }

        /* # ZADANIE 11 - 14: Detekcja obiektow i bryly */
        yolo.detect(obraz_z_kamery, Size(800, 800));

        for (int klasa : {0, 2}) { /* # 0 - czlowiek, 2 - samochod */
            for (Rect wykryty_obiekt : yolo.valid_boxes[klasa]) {
                
                float szer = (klasa == 2) ? 1.8f : 0.6f;
                float dlug = (klasa == 2) ? 4.5f : 0.6f;
                float wys  = (klasa == 2) ? 1.5f : 1.8f;
                Scalar kol = (klasa == 2) ? Scalar(0, 255, 0) : Scalar(255, 0, 255);

                Point2f lewy_dolny, prawy_dolny, prawy_gorny, lewy_gorny;
                float odleglosc_od_obiektu = 0.0f;

                /* # Sprawdzenie polowy ekranu w celu wybrania lepszego naroznika punktu styku */
                if (wykryty_obiekt.x + wykryty_obiekt.width / 2.0f > obraz_z_kamery.cols / 2.0f) {
                    /* # Obiekt po prawej - punktem referencyjnym jest lewy dolny rog ramki */
                    lewy_dolny = transformuj_wspolrzedne(Point2f(wykryty_obiekt.x, wykryty_obiekt.y + wykryty_obiekt.height), H2.inv());
                    prawy_dolny = lewy_dolny + Point2f(szer, 0);
                    lewy_gorny = lewy_dolny + Point2f(0, dlug);
                    prawy_gorny = prawy_dolny + Point2f(0, dlug);
                    odleglosc_od_obiektu = lewy_dolny.y;
                } else {
                    /* # Obiekt po lewej - punktem referencyjnym jest prawy dolny rog ramki */
                    prawy_dolny = transformuj_wspolrzedne(Point2f(wykryty_obiekt.x + wykryty_obiekt.width, wykryty_obiekt.y + wykryty_obiekt.height), H2.inv());
                    lewy_dolny = prawy_dolny - Point2f(szer, 0);
                    lewy_gorny = lewy_dolny + Point2f(0, dlug);
                    prawy_gorny = prawy_dolny + Point2f(0, dlug);
                    odleglosc_od_obiektu = prawy_dolny.y;
                }

                /* # Rysowanie obrysu na widoku z lotu ptaka */
                line(wyprostowany, transformuj_wspolrzedne(lewy_dolny, H1 * H2), transformuj_wspolrzedne(prawy_dolny, H1 * H2), kol, 2);
                line(wyprostowany, transformuj_wspolrzedne(lewy_dolny, H1 * H2), transformuj_wspolrzedne(lewy_gorny, H1 * H2), kol, 2);
                line(wyprostowany, transformuj_wspolrzedne(lewy_gorny, H1 * H2), transformuj_wspolrzedne(prawy_gorny, H1 * H2), kol, 2);
                line(wyprostowany, transformuj_wspolrzedne(prawy_dolny, H1 * H2), transformuj_wspolrzedne(prawy_gorny, H1 * H2), kol, 2);

                /* # Przeliczenie do przestrzeni 3D za pomoca H3 i projectPoints */
                vector<Point2f> podstawa = { lewy_dolny, prawy_dolny, prawy_gorny, lewy_gorny };
                vector<Point3f> p_dolna3D, p_gorna3D;
                
                for (auto& p : podstawa) {
                    Point2f p_cm = transformuj_wspolrzedne(p, H3);
                    p_dolna3D.push_back(Point3f(p_cm.x, p_cm.y, 0));
                    /* # Jesli uklad wspolrzednych rysuje auta "pod jezdnia", zmien znak przy wys: -wys * 100.0f */
                    p_gorna3D.push_back(Point3f(p_cm.x, p_cm.y, wys * 100.0f)); 
                }

                vector<Point2f> p_dolna2D, p_gorna2D;
                projectPoints(p_dolna3D, rot, trans, macierzKamery, wspolczynnikiZnieksztalcen, p_dolna2D);
                projectPoints(p_gorna3D, rot, trans, macierzKamery, wspolczynnikiZnieksztalcen, p_gorna2D);

                for (int i = 0; i < 4; i++) {
                    line(obraz_z_kamery, p_dolna2D[i], p_dolna2D[(i + 1) % 4], kol, 2);
                    line(obraz_z_kamery, p_gorna2D[i], p_gorna2D[(i + 1) % 4], kol, 2);
                    line(obraz_z_kamery, p_dolna2D[i], p_gorna2D[i], kol, 2);
                }

                /* # Wypisanie odleglosci nad obiektem */
                Point2f punkt_wypisania = Point2f((p_dolna2D[1].x + p_gorna2D[3].x) / 2, (p_dolna2D[1].y + p_gorna2D[3].y) / 2);
                putText(obraz_z_kamery, format("%.1f m", odleglosc_od_obiektu), punkt_wypisania, 0, 1, CV_RGB(255, 255, 255), 2);
            }
        }

        imshow("Obraz z kamery", obraz_z_kamery);
        imshow("Widok z lotu ptaka", wyprostowany);
    }
    return 0;
}
