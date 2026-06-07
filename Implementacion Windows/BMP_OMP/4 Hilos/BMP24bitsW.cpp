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
#include <omp.h>
#include "BMP.h"

#define NUM_HILOS 4

extern "C" void brightness_xmmw(unsigned char u[16], BYTE* gimg, int size);

int main(int argc, char** argv)
{
	LARGE_INTEGER tinicio, tfin, frec;
	double mcseg;
	unsigned char u[16] = { 50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50 };
	int i;
	BMP bmp("nature_mediana.bmp");	
	//BMP bmp("nature_grande.bmp");
	//BMP bmp("nature_muy_grande.bmp");

	BYTE* base_img = bmp.getImageData();
	int total_size = bmp.getImageSize();
	int chunck_size = total_size / NUM_HILOS;

	QueryPerformanceFrequency(&frec);
	QueryPerformanceCounter(&tinicio);
	
	omp_set_num_threads(NUM_HILOS);

#pragma omp parallel for private(i)
	for(i = 0; i < NUM_HILOS; i++) {
		BYTE* gimp_ptr = base_img + (chunck_size * i);
		brightness_xmmw(u, gimp_ptr, chunck_size);
	}

	QueryPerformanceCounter(&tfin);

	mcseg = (double)(tfin.QuadPart - tinicio.QuadPart);
	mcseg = mcseg / frec.QuadPart;
	printf("%lf\n", mcseg * 1000);

	bmp.save("nature_sec_50.bmp");

	return 0;
}