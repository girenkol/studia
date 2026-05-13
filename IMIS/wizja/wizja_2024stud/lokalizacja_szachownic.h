#pragma once

#include "opencv2\opencv.hpp"
#include "szachownica.h"


extern Mat obraz_z_kamery;

using namespace cv;
using namespace std;
using namespace dnn;


int lokalizacja_szachownic()
{

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
	//panel do ustawienia ostrosci - po sprawdzeniu, ze jest dobrze, mozna zakomentowac
	kamera.set(CAP_PROP_SETTINGS, -1);

	Szachownica szachownica(5, 6, 20); //

	while (waitKey(1) != 27)
	{
		kamera >> obraz_z_kamery;


		Mat Tks = szachownica.wyznacz_poze(obraz_z_kamery); //Tks - polozenie szachownicy w ukladzie kamery
		printf(Tks);

		//to samo dla drugiej szachownicy
			//potem wyznaczyc polozenie malej szachownicy wzgledem duzej

		vector<Point3f> p{ {60,60,0},{60,60,100} }; //punkty w trzech wymiarach
		vector<Point2f> p2; //te same punkty na ekranie (w pikselach) - obliczyæ je funkcj¹ projectPoints i narysowaæ



		imshow("obraz", obraz_z_kamery);
	}


	return 0;
}
