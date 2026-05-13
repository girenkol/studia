#pragma once

#include "opencv2\opencv.hpp"




using namespace cv;
using namespace std;

extern Mat obraz_z_kamery;
extern Mat macierzKamery;
extern Mat wspolczynnikiZnieksztalcen;


class Szachownica
{
private:
	Size uklad;
	double wymiar;
	vector<Point2f> narozniki;


public:
	Mat T, rot, trans;

	//szachownice definiuje liczna punktow styku czarnych kwadratow w obu osiach oraz dlugosc boku kwadratu
	Szachownica(int szer, int wys, double wymiar)
	{
		this->uklad = Size(szer, wys);
		this->wymiar = wymiar;
	}
	

	Mat wyznacz_poze(Mat& obraz, bool rysuj = true)
	{
		bool szachownica_znaleziona1 = findChessboardCorners(obraz, uklad, narozniki,/*CALIB_CB_ADAPTIVE_THRESH | */CALIB_CB_NORMALIZE_IMAGE | CALIB_CB_FAST_CHECK);
		if (szachownica_znaleziona1 == true)
		{
			//tym razem mozemy je dokladniej zlokalizowac dzieki tej funkcji
			Mat tmp;
			cvtColor(obraz_z_kamery, tmp, COLOR_BGR2GRAY);
			cornerSubPix(tmp, narozniki, Size(10, 10), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 400, 0.001));

			//wygenerowanie lokalnych wspolrzednych wewnetrznych punktow szachownicy
			vector<Point3f>  punkty_na_szachownicy1;
			for (int i = 0; i < uklad.height; i++)
				for (int j = 0; j < uklad.width; j++)
					punkty_na_szachownicy1.push_back(Point3f(float(i * wymiar), float(j * wymiar), 0));

			solvePnP(punkty_na_szachownicy1, narozniki, macierzKamery, wspolczynnikiZnieksztalcen, rot, trans);

			if (rysuj)
			{
				drawChessboardCorners(obraz, uklad, narozniki, 1);
				drawFrameAxes(obraz_z_kamery, macierzKamery, wspolczynnikiZnieksztalcen, rot, trans, 2 * wymiar);
			}

			//tworzenie macierzy przeksztalcenia uogolnionego
			T = Mat::eye(4, 4, CV_64F);
			Rodrigues(rot, T(Rect(0, 0, 3, 3)));
			Mat(trans).copyTo(T(Rect(3, 0, 1, 3)));

			return(T);
		}
		return(Mat::zeros(4, 4, CV_64F)); //w razie niepowodzenia zwracamy macierz zerow?

	}
};
