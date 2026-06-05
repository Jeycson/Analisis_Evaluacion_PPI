/*
Cargar y procesar imágenes BMP de 24bits con C++
http://www.widget-101.com/
Autor: Christiam Mena
*/

#include <stdio.h>  
#include <string.h>
#include <Windows.h>
#include <omp.h>
#include "BMP.h"

#define NUMH 16

struct stDatos {
    unsigned char u[16];
    BYTE* gimg;
    int size;
};

typedef struct stDatos STDATOS;

extern "C" void brightness_xmmw(unsigned char u[16], BYTE* gimg, int size);

DWORD WINAPI hilos(LPVOID param) {
    STDATOS* datos = (STDATOS*)param;
    brightness_xmmw(datos->u, datos->gimg, datos->size);
    return 0;
}

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

	//BMP bmp("nature_mediana.bmp");	
	//BMP bmp("nature_grande.bmp");
	BMP bmp("nature_muy_grande.bmp");
	//bmp.printHeader();

	BYTE* base_img = bmp.getImageData();
	int total_size = bmp.getImageSize();
	int chunck_size = total_size / NUMH;

	QueryPerformanceFrequency(&frec);
	QueryPerformanceCounter(&tinicio);
	omp_set_num_threads(NUMH);

	//bmp.brightness(50);
	//brightness_xmmw(u, bmp.getImageData(), bmp.getImageSize());
	/*
	for (i = 0; i < NUMH; i++) {
		datos[i].size = bmp.getImageSize() / NUMH;
		datos[i].gimg = bmp.getImageData() + (sizeof(BYTE) * i * datos[i].size);
		memcpy(datos[i].u, u, 16);
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hilos, &datos[i], 0, dwThreadId[i]);

		if (!hThread[i]) {
			printf("Error al crear el hilo %d/n", i);
			return -1;
		}
	}

	for (i = 0; i < NUMH; i++) {
		WaitForSingleObject(hThread[i], INFINITE);
		CloseHandle(hThread[i]);
	}
    */

#pragma omp parallel for
	for (int i = 0; i < NUMH; i++) {
		BYTE* gimp_ptr = base_img + (chunck_size * i);
		brightness_xmmw(u, gimp_ptr, chunck_size);
	}

	QueryPerformanceCounter(&tfin);

	mcseg = (double)(tfin.QuadPart - tinicio.QuadPart);
	mcseg = mcseg / frec.QuadPart;
	printf("%lf\n", mcseg * 1000);


	bmp.save("nature_asm_50.bmp");

	//getchar();  // Con esta línea evitaremos que se cierre la consola
	return 0;
}