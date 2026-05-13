#pragma once

#include "opencv2\opencv.hpp"

using namespace cv;
using namespace std;

vector<Point2f> punkty_na_obrazie;
vector<Point2f> H_punkty_klikniete;
vector<Point2f> H_punkty_przeliczone;
Mat obraz_z_kamery;

Point2f transformuj_wspolrzedne(Point2f samochodowe, Mat h)
{

	Point3d p1(samochodowe.x, samochodowe.y, 1);
	Point3d p2 = Point3d(Mat(h * Mat(p1))); //to jest po prostu H*p1, ale w OpenCV trzeba pomanipulowac typami danych
	p2 /= p2.z;
	return(Point2f(p2.x, p2.y));

}
Point2f transformuj_wspolrzedne(float x, float y, Mat h)
{

	Point3d p1(x, y, 1);
	Point3d p2 = Point3d(Mat(h * Mat(p1))); //to jest po prostu H*p1, ale w OpenCV trzeba pomanipulowac typami danych
	p2 /= p2.z;
	return(Point2f(p2.x, p2.y));

}

Vec3f rotationMatrixToEulerAngles(Mat& R)
{


	float sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

	bool singular = sy < 1e-6; // If

	float x, y, z;
	if (!singular)
	{
		x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
		y = atan2(-R.at<double>(2, 0), sy);
		z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
	}
	else
	{
		x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
		y = atan2(-R.at<double>(2, 0), sy);
		z = 0;
	}
	return Vec3f(x, y, z);

}

//pobiera wspolrzedne obrazowe kliknietego punktu i jesli ostatni parametr jest true, rysuje kolko na obraz_z_kamery
//wspolrzedne punktu zapisywane sa na kolejnych pozycjach wektora punkty_na_obrazie
void klikniecie_mysza(int event, int x, int y, int, void* parametr) {
	if (event == EVENT_LBUTTONDOWN)
	{
		//klikniety punkt wpisywany jest do wektora punktow
		Point p(x, y);
		punkty_na_obrazie.push_back(p);
		cout << p << endl;

		if ((bool)parametr == true)
		{
			circle(obraz_z_kamery, p, 5, Scalar(0, 0, 255));
			imshow("obraz", obraz_z_kamery);
			waitKey(1);
		}
	}
}

//pobiera wspolrzedne obrazowe kliknietego punktu i PRZELICZA je na wspolrzedne obiektu na podstawie homografii
//wspolrzedne punktu zapisywane sa na kolejnych pozycjach wektora H_punkty_przeliczone
void klikniecie_mysza_z_homografia(int event, int x, int y, int, void* parametr) {
	//przelicza wspolrzedne kliknietego punktu na obrazie oryginalnym 
	//na wspolrzedne na docelowej plaszczyznie
	Mat* H = (Mat*)parametr;
	if (event == EVENT_LBUTTONDOWN)
	{
		// klikniety punkt
		Point3d p1(x, y, 1);
		Point3d p2 = Point3d(Mat(*H * Mat(p1))); //to jest po prostu H*p1, ale w OpenCV trzeba pomanipulowaæ typami danych
		p2 /= p2.z;
		H_punkty_klikniete.push_back(Point2f(p1.x, p1.y));
		H_punkty_przeliczone.push_back(Point2f(p2.x, p2.y));

		cout << "Wsp. na obrazie: " << Point2f(p1.x, p1.y) << " na obiekcie: " << Point2f(p2.x, p2.y) << endl;


	}
}

//rozszerza funkcjonalnosc standrdowego polecenia imshow o dodatkowy napis na obrazie, co bywa przydatne
void imshow(string okno, Mat& obraz, string tekst, double rozmiar = 2)
{
	rectangle(obraz, Rect(0, 0, 800, 40), CV_RGB(150, 150, 150), -1);
	putText(obraz, tekst, Point(10, 30), cv::FONT_HERSHEY_PLAIN, rozmiar, CV_RGB(155, 0, 0), 2.5);
	putText(obraz, tekst, Point(10, 30), cv::FONT_HERSHEY_PLAIN, rozmiar, CV_RGB(255, 0, 0), 1);
	imshow(okno, obraz);
}

//wypisuje macierz w wygodnej do odczytu formie
void printf(Mat mat, String opis = " ", int total = 6, int decimal = 1)
{
	if (opis != " ")
		cout << opis << endl;
	for (int i = 0; i < mat.size().height; i++)
	{
		printf("|");
		for (int j = 0; j < mat.size().width; j++)
		{
			printf("%*.*f ", total, decimal, mat.at<double>(i, j));
			if (j == mat.size().width - 1)
				printf(" |\n");
		}
	}
	printf("\n");
}



#include "homografia.h"
#include "szachownica.h"
#include "yolo.h"
#include "yolo_demo.h"
#include "lokalizacja_szachownic.h"
#include "lokalizacja_3D.h"
#include "ulica.h"

