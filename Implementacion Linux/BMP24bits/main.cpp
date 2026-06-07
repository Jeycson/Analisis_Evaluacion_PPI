/*
Cargar y procesar imágenes BMP de 24bits con C++
Adaptado para Linux (Fedora/Ubuntu)
*/

#include <stdio.h>       
#include <iostream>
#include <cstring>     
#include <thread>       
#include <vector>
#include <chrono>      
#include <omp.h>
#include "BMP.h"        

#define NUM 16

struct stDAtos
{
    unsigned char u[16];
    BYTE* gimg;
    int size;
};
typedef struct stDAtos STDATOS;

extern "C" void brightness_xmml(unsigned char u[16], BYTE* gimg, int size);

void hilo(STDATOS* datos);

// ============================================================================
// ENFOQUE 1: SECUENCIAL 
// ============================================================================
void procesar_secuencial(BMP& bmp) {
    bmp.brightness(50);
}

// ============================================================================
// ENFOQUE 2: SIMD (Hilos portables en Linux)
// ============================================================================
void procesar_paralelo_simd(BMP& bmp, unsigned char u[16]) {
    std::vector<std::thread> hilos;
    STDATOS datos[NUM];

    for (int i = 0; i < NUM; i++)
    {
        // Corregido a getPImageData()
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
}

void hilo(STDATOS* datos)
{
    brightness_xmml(datos->u, datos->gimg, datos->size);
}

// ============================================================================
// ENFOQUE 3: PARALELO CON OPENMP (OMP)
// ============================================================================
void procesar_paralelo_omp(BMP& bmp) {
    BYTE* data = bmp.getPImageData();
    int size = bmp.getImageSize();

#pragma omp parallel for schedule(static) num_threads(NUM)
    for (int i = 0; i < size; i++) {
        int temp = data[i] + 50;
        if (temp > 255) data[i] = 255;
        else if (temp < 0) data[i] = 0;
        else data[i] = (BYTE)temp;
    }
}

int main(int argc, char** argv)
{
    unsigned char u[16] = { 50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50 };
    BMP bmp("nature2.bmp"); 

    auto tinicio = std::chrono::high_resolution_clock::now();

    // ENFOQUE 1: Secuencial
    // procesar_secuencial(bmp);

    // ENFOQUE 2: Paralelo SIMD (Descoméntalo cuando quieras probarlo)
    // procesar_paralelo_simd(bmp, u);

    // ENFOQUE 3: Paralelo con OpenMP (Descoméntalo cuando quieras probarlo)
     procesar_paralelo_omp(bmp);

    auto tfin = std::chrono::high_resolution_clock::now();
    bmp.save("brightness.bmp");

    std::chrono::duration<double, std::milli> tiempo = tfin - tinicio;
    printf("Tiempo transcurrido: %lf ms\n", tiempo.count());

    return 0;
}
