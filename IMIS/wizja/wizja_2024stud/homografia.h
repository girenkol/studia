#pragma once
#ifndef homografia
#define homografia
#include "opencv2\opencv.hpp"
#include "pomocnicze.h"



using namespace cv;
using namespace std;

extern Mat obraz_z_kamery;



int homografia_z_kartka()
{
	//homografia - przeksztalcenie plaszczyzny w plaszczyzne
	//nalezy wskazac 4 punkty na obrazie obserwowanej plaszczyzny (wspolrzedne obrazowe)
	//oraz ich wspolrzedne w lokalnym ukladzie tejze plaszczyzny (np. kartki A4)
	//parametry kamery sa nieistotne (o ile nie znieksztalca za bardzo obrazu)

	//inicjacja kamery (pierwsza kamera w systemie ma nr 0)
	VideoCapture kamera(0, CAP_DSHOW); //CAP_DSHOW moze przyspieszyc inicjacje kamery, ale i zmienic jej numer

	if (!kamera.isOpened())
	{
		cout << "\n\nNie mozna polaczyc sie z kamera, moze jej nie ma, a moze jest uzywana przez inny program? \n\nNacisnij ENTER by zakonczyc program!";
		cin.get();
		return 0;
	}
	//ustalenie rozdzielczosci
	kamera.set(CAP_PROP_FRAME_WIDTH, 1280);
	kamera.set(CAP_PROP_FRAME_HEIGHT, 720);


	//petla pozwalajaca na ustawienie kamery, po ESC nastepuje faza klikania

	while (waitKey(1) != 27) //jesli nie nacisnieto ESC obraz jest pobierany i wyswietlany w petli
	{
		kamera >> obraz_z_kamery;
		if (obraz_z_kamery.data == NULL) break; //zabezpieczenie - jesli nie ma nowej klatki, przerywa dzialanie
		putText(obraz_z_kamery, String("Obraz na zywo...nacisnij ESC by zatrzymac"), Point(0, 25), 0, 1, CV_RGB(255, 0, 0), 2);
		imshow("obraz", obraz_z_kamery);
	}


	//po nacisnieciu ESC
	kamera >> obraz_z_kamery; //pobranie swiezego obrazu z kamery (na poprzednim jest napis)
	Mat kopia = obraz_z_kamery.clone(); //kopia obrazu z kamery - przyda sie nizej kopia czystego obrazu, bez napisow i linii
	putText(obraz_z_kamery, String("Nacisnieto ESC, obraz zatrzymany!"), Point(0, 25), 0, 1, CV_RGB(255, 0, 0), 2);
	imshow("obraz", obraz_z_kamery);

	//jesli nie ma kamery, ladowanie obrazu z pliku
	//obraz_z_kamery = imread("kartka1.jpg");
	//imshow("obraz", obraz_z_kamery);
	//waitKey(1);


	//ustawienie funkcji czytajacej mysz na danym oknie z obrazem (true oznacza, ze rysowane sa koleczka w miejscu klikniecia)
	setMouseCallback("obraz", klikniecie_mysza, (void*)true);
	//wyswietlenie obrazu wraz z napisem na tym obrazie
	imshow("obraz", obraz_z_kamery, "Oznacz 4 punkty na plaszczyznie", 1.5);
	waitKey(1);

	while (punkty_na_obrazie.size() < 4) waitKey(1); //poczekanie na 4 punkty_na_obrazie

	//narysowanie wielokata laczacego punkty_na_obrazie
	for (int i = 0; i < 4; i++)
		line(obraz_z_kamery, punkty_na_obrazie[i], punkty_na_obrazie[(i + 1) % 4], CV_RGB(0, 255, 100), 1);


	//wygenerowanie lokalnych wspolrzednych X,Y zaznaczonych naroznikow
	//punkty_na_obrazie byly zaznaczane w takiej kolejnosci, by odpowiadaly ponizszym wspolrzednym punktow na obiekcie

	vector<Point2f>  punkty_na_obiekcie{ { 0, 0 },{ 210, 0 },{ 210, 297 },{ 0, 297 } }; //kartka A4

	
	Mat H = findHomography(punkty_na_obrazie, punkty_na_obiekcie);

	printf(H, "Macierz przeksztalcenia homograficznego = ");

	Mat obraz2;
	warpPerspective(kopia, obraz2, H, Size(210, 297)); //prostujemy wczesniej zrobiona kopie czystego obrazu
	imshow("wyprostowany", obraz2);
	waitKey(1);



	//odczyt wspolrzednych punktu na oznaczonej plaszczyznie
	//z obrazu wyprostowanego
	setMouseCallback("wyprostowany", klikniecie_mysza);
	punkty_na_obrazie.clear();

	//z obrazu oryginalnego
	H_punkty_klikniete.clear();
	H_punkty_przeliczone.clear();
	setMouseCallback("obraz", klikniecie_mysza_z_homografia, (void*)&H);

	while (waitKey(1) != 27)
	{
		//na razie ta petla nic nie robi, tylko pokazuje obraz

		imshow("obraz", obraz_z_kamery);


	}

	return 0;
}


#endif