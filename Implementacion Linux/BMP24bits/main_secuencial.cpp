/*
   Enfoque 1: SECUENCIAL
   Cargar y procesar imágenes BMP de 24bits con C++
*/

#include <stdio.h>        
#include <iostream>
#include <chrono>      
#include "BMP.h"        

int main(int argc, char** argv)
{
    BMP bmp("nature2.bmp"); 

    auto tinicio = std::chrono::high_resolution_clock::now();

    // Proceso secuencial nativo de la clase BMP
    bmp.brightness(50);

    auto tfin = std::chrono::high_resolution_clock::now();
    bmp.save("brightness_secuencial.bmp");

    std::chrono::duration<double, std::milli> tiempo = tfin - tinicio;
    printf("[SECUENCIAL] Tiempo transcurrido: %lf ms\n", tiempo.count());

    return 0;
}
