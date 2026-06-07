/*
Cargar y procesar imágenes BMP de 24bits con C++
http://www.widget-101.com/
Autor: Christiam Mena
Editado por: 
Jeycson Gabriel López Hernández
Jeovani Pacheco Rueda
Mariana Palacios Trinidad
Jairo Iván Hipolito Morales
*/

#include <stdio.h>  
#include <string.h>
#include <Windows.h>
#include "BMP.h"

extern "C" void brightness_xmmw(unsigned char u[16], BYTE* gimg, int size);

int main(int argc, char** argv)
{
	LARGE_INTEGER tinicio, tfin, frec;
	double mcseg;
	unsigned char u[16] = { 50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50 };

	BMP bmp("nature_mediana.bmp");	
	//BMP bmp("nature_grande.bmp");
	//BMP bmp("nature_muy_grande.bmp");

	QueryPerformanceFrequency(&frec);
	QueryPerformanceCounter(&tinicio);
	
	brightness_xmmw(u, bmp.getImageData(), bmp.getImageSize());
	
	QueryPerformanceCounter(&tfin);

	mcseg = (double)(tfin.QuadPart - tinicio.QuadPart);
	mcseg = mcseg / frec.QuadPart;
	printf("%lf\n", mcseg * 1000);

	bmp.save("nature_sec_50.bmp");

	return 0;
}