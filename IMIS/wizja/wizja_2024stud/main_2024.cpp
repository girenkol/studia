#include "pomocnicze.h"


Mat macierzKamery = (Mat_<double>(3, 3) << 1745.99, 0, 968.8, 0, 1746.17, 528.09, 0, 0, 1); //UZUPELNIC
Mat wspolczynnikiZnieksztalcen = (Mat_<double>(1, 5) << 0.2769, -0.7041, -0.00176, -0.002547, 0);//UZUPELNIC, kolejność: 2 radialne, 2 tangencjalne i zero



int main()
{
	
	//homografia_z_kartka();
	//lokalizacja_3D();
	//lokalizacja_szachownic();
	ulica();
	
	return 0;
}


