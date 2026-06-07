/*
   Enfoque 3: PARALELO CON OPENMP (OMP)
   Cargar y procesar imágenes BMP de 24bits con C++
*/

#include <stdio.h>        
#include <iostream>
#include <chrono>      
#include <omp.h>
#include "BMP.h"        

int main(int argc, char** argv)
{
    BMP bmp("nature2.bmp"); 

    auto tinicio = std::chrono::high_resolution_clock::now();

    BYTE* data = bmp.getPImageData();
    int size = bmp.getImageSize();

    // Paralelización del ciclo con OpenMP
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < size; i++) {
        int temp = data[i] + 50;
        if (temp > 255) data[i] = 255;
        else if (temp < 0) data[i] = 0;
        else data[i] = (BYTE)temp;
    }

    auto tfin = std::chrono::high_resolution_clock::now();
    bmp.save("brightness_omp.bmp");

    std::chrono::duration<double, std::milli> tiempo = tfin - tinicio;
    printf("[OpenMP] Tiempo transcurrido: %lf ms\n", tiempo.count());

    return 0;
}
