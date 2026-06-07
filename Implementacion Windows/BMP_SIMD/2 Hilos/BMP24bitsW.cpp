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

#define NUM_HILOS 2

struct stDatos {
    unsigned char u[16];
    BYTE* gimg;
    int size;
};

typedef struct stDatos STDATOS;
extern "C" void brightness_xmmw(unsigned char u[16], BYTE* gimg, int size);
DWORD WINAPI hilo(LPVOID pDatos);

int main(int argc, char** argv)
{
	LARGE_INTEGER tinicio, tfin, frec;
	double mcseg;
	unsigned char u[16] = { 50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50,
							50, 50, 50, 50 };

	//Variables para el manejo de hilos
	HANDLE hThread[NUM_HILOS] = { NULL };
	LPDWORD dThreadId = { NULL };
	STDATOS datos[NUM_HILOS];
	int i;

	BMP bmp("nature_mediana.bmp");	
	//BMP bmp("nature_grande.bmp");
	//BMP bmp("nature_muy_grande.bmp");

	QueryPerformanceFrequency(&frec);
	QueryPerformanceCounter(&tinicio);
	
	for(i = 0; i < NUM_HILOS; i++) {
		datos[i].gimg = bmp.getImageData() + (sizeof(BYTE) * (bmp.getImageSize() / NUM_HILOS) * i);
		datos[i].size = bmp.getImageSize() / NUM_HILOS;
		memcpy(datos[i].u, u, 16);
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hilo, &datos[i], 0, dThreadId);

		if(hThread[i] == NULL) {
			printf("Error al crear el hilo %d\n", i);
			return 1;
		}	
	}

	for (i = 0; i < NUM_HILOS; i++) {
		WaitForSingleObject(hThread[i], INFINITE);
		CloseHandle(hThread[i]);
	}
	
	QueryPerformanceCounter(&tfin);

	mcseg = (double)(tfin.QuadPart - tinicio.QuadPart);
	mcseg = mcseg / frec.QuadPart;
	printf("%lf\n", mcseg * 1000);

	bmp.save("nature_sec_50.bmp");

	return 0;
}

DWORD WINAPI hilo(LPVOID pDatos) {
	STDATOS* datos = (STDATOS*)pDatos;
	brightness_xmmw(datos->u, datos->gimg, datos->size);
	return 0;
}