/*
Cargar y procesar imágenes BMP de 24bits con C++
http://www.widget-101.com/
Autor: Christiam Mena
*/

#include <stdio.h>  
#include <string.h>
#include <Windows.h>
#include "BMP.h"

#define NUMH 16

struct stDatos {
    unsigned char u[16];
    BYTE* gimg;
    int size;
};

typedef struct stDatos STDATOS;

int main(int argc, char** argv)
{
	LARGE_INTEGER tinicio, tfin, frec;
	double mcseg;
	unsigned char u[16] = { 50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50 };
	
	HANDLE hThread[NUMH] = { NULL };
	LPDWORD dwThreadId[NUMH] = { NULL };
	STDATOS datos[NUMH];
	int i;

	BMP bmp("nature_mediana.bmp");	
	//BMP bmp("nature_grande.bmp");
	//BMP bmp("nature_muy_grande.bmp");

	BYTE* base_img = bmp.getImageData();
	int total_size = bmp.getImageSize();
	int chunck_size = total_size / NUMH;

	QueryPerformanceFrequency(&frec);
	QueryPerformanceCounter(&tinicio);
	
	bmp.brightness(50);
	
	QueryPerformanceCounter(&tfin);

	mcseg = (double)(tfin.QuadPart - tinicio.QuadPart);
	mcseg = mcseg / frec.QuadPart;
	printf("%lf\n", mcseg * 1000);

	bmp.save("nature_sec_50.bmp");

	//getchar();  // Con esta línea evitaremos que se cierre la consola
	return 0;
}