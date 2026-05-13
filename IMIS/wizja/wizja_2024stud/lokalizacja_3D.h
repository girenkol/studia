#pragma once

#include "opencv2\opencv.hpp"



extern Mat obraz_z_kamery;

using namespace cv;
using namespace std;




int lokalizacja_3D()
{	//lokalizacja obiektu w przestrzeni 3D
	//przynajmniej 4 punkty i ich polozenie w lokalnym ukladzie obiektu musi byc znane
	//niezbedne sa takze parametry kamery

	//podlaczenie i ustawienie rozdzielczosci kamery
	VideoCapture kamera(0, CAP_DSHOW);
	if (!kamera.isOpened())
	{
		cout << "\n\nNie mozna polaczyc sie z kamera, moze jej nie ma, a moze jest uzywana przez inny program? \n\nNacisnij ENTER by zakonczyc program!";
		cin.get();
		return 0;
	}
	//ustalenie rozdzielczosci
	kamera.set(CAP_PROP_FRAME_WIDTH, 1280);
	kamera.set(CAP_PROP_FRAME_HEIGHT, 720);
	kamera.set(CAP_PROP_SETTINGS, -1);


	while (waitKey(1) != 27) //jesli nie nacisnieto ESC obraz jest pobierany i wyswietlany w petli
	{
		kamera >> obraz_z_kamery;
		if (obraz_z_kamery.data == NULL) break; //zabezpieczenie - jesli nie ma nowej klatki, przerywa dzialanie
		imshow("obraz", obraz_z_kamery, "Obraz na zywo...nacisnij ESC by zatrzymac");
	}

	//po nacisnieciu ESC
	kamera >> obraz_z_kamery; //pobranie swiezego obrazu z kamery (na poprzednim jest napis)
	setMouseCallback("obraz", klikniecie_mysza, (void*)true); //ustawienie funkcji czytajacej mysz na danym oknie z obrazem (true oznacza, ze rysowane sa koleczka w miejscu klikniecia)

	imshow("obraz", obraz_z_kamery, "Nacisnieto ESC, obraz zatrzymany!");
	putText(obraz_z_kamery, String("Oznacz 4 punkty na obiekcie"), Point(0, 55), 0, 1, CV_RGB(255, 0, 0), 2);
	imshow("obraz", obraz_z_kamery);
	waitKey(1);
	punkty_na_obrazie.clear();
	while (punkty_na_obrazie.size() < 4) waitKey(1); //poczekanie na 4 punkty_na_obrazie


	//narysowanie wielokata laczacego punkty_na_obrazie
	for (int i = 0; i < 4; i++)
		line(obraz_z_kamery, punkty_na_obrazie[i], punkty_na_obrazie[(i + 1) % 4], CV_RGB(0, 255, 100), 2);


	//wygenerowanie lokalnych wspolrzednych zaznaczonych naroznikow
	//kolejnosc klikania punktow na obrazie musi byc taka jak kolejnosc podana ponizej!

	vector<Point3f>  punkty_na_obiekcie{ { 0, 0, 0 },{ 210, 0, 0 },{ 210, 297, 0 },{ 0, 297, 0 } }; // tym razem wspolrzedne 3D!

	Mat trans, rot; //wektory translacji i rotacji obiektu
	solvePnP(punkty_na_obiekcie, punkty_na_obrazie, macierzKamery, wspolczynnikiZnieksztalcen, rot, trans);
	printf(trans, "\nWspolrzedne obiektu (poczatku jego lokalnego ukladu wspolrzednych w ukladzie kamery):");

	//tworzenie macierzy przeksztalcenia uogolnionego - polozenie obiektu w ukladzie kamery
	Mat T_ko = Mat::eye(4, 4, CV_64F); //macierz jednostkowa
	Rodrigues(rot, T_ko(Rect(0, 0, 3, 3))); //przeksztalcenie wektora rotacji na klasyczna macierz rotacji i skopiowanie jej do macierzy transformacji uogolnionej
	Mat(trans).copyTo(T_ko(Rect(3, 0, 1, 3))); //skopiowanie wektora translacji do macierzy T
	printf(T_ko, "Macierz przeksztalcenie uogolnionego (polozenie obiektu w ukladzie kamery):");



	imshow("obraz", obraz_z_kamery, "Nacisnij ESC by zakonczyc program");
	waitKey(); //dowolny klawisz konczy program

	//do zrobienia
	//1. Narysowaæ kó³ko w œrodku obrazu
	//2. Ustawiæ kartkê A4 tak, aby jej pierwszy róg by³ w œrodku obrazu i wyznaczyæ translacjê wzd³u¿ osi Z - sprawdziæ linik¹
	//czy siê mniej wiêcej zgadza
	//3. Zweryfikowaæ z innym obiektem o znanych wymiarach
	//4. Zdefiniowaæ drug¹ kamerê z uwzglêdnieniem parametrów z kalibracji i porównaæ wyniki z kalibracji na oko i z tej poprawnej
	return 0;
}
