/*
   Enfoque 2: SIMD (Hilos portables en Linux)
   Cargar y procesar imágenes BMP de 24bits con C++
*/

#include <stdio.h>        
#include <iostream>
#include <cstring>     
#include <thread>       
#include <vector>
#include <chrono>      
#include "BMP.h"        

#define NUM 2

struct stDAtos
{
    unsigned char u[16];
    BYTE* gimg;
    int size;
};
typedef struct stDAtos STDATOS;

extern "C" void brightness_xmml(unsigned char u[16], BYTE* gimg, int size);

void hilo(STDATOS* datos)
{
    brightness_xmml(datos->u, datos->gimg, datos->size);
}

int main(int argc, char** argv)
{
    unsigned char u[16] = { 50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50 };
    BMP bmp("nature2.bmp"); 

    auto tinicio = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> hilos;
    STDATOS datos[NUM];

    for (int i = 0; i < NUM; i++)
    {
        datos[i].gimg = bmp.getPImageData() + (sizeof(BYTE) * (bmp.getImageSize() / NUM) * i);
        datos[i].size = bmp.getImageSize() / NUM;
        std::memcpy(datos[i].u, u, 16);
        
        hilos.push_back(std::thread(hilo, &datos[i]));
    }

    for (int i = 0; i < NUM; i++)
    {
        if (hilos[i].joinable()) {
            hilos[i].join();
        }
    }

    auto tfin = std::chrono::high_resolution_clock::now();
    bmp.save("brightness_simd.bmp");

    std::chrono::duration<double, std::milli> tiempo = tfin - tinicio;
    printf("[SIMD + THREADS] Tiempo transcurrido: %lf ms\n", tiempo.count());

    return 0;
}
