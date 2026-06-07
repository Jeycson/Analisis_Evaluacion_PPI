import cv2
import numpy as np
import time
import tkinter as tk
from tkinter import filedialog
import matplotlib.pyplot as plt
import threading

def seleccionar_imagen():
    root = tk.Tk()
    root.withdraw()
    ruta_imagen = filedialog.askopenfilename(
        title="Selecciona una imagen BMP de 24 bits",
        filetypes=[("Archivos BMP", "*.bmp"), ("Todos los archivos", "*.*")]
    )
    return ruta_imagen

def procesar_bloque_brillo(img_original, inicio_f, fin_f, vector_brillo, img_resultado):
    porcion = img_original[inicio_f:fin_f, :]
    img_resultado[inicio_f:fin_f, :] = cv2.add(porcion, vector_brillo)

def ejecutar_corrida_completa(ruta_imagen, num_hilos, factor_brillo=50):
    """
    Ejecuta UNA corrida completa midiendo desde la lectura de disco 
    hasta la escritura final (extremo a extremo).
    """
    t_inicio = time.perf_counter()
    
    # 1. Carga de disco
    img = cv2.imread(ruta_imagen)
    if img is None:
        return None
        
    alto, ancho, _ = img.shape
    vector_brillo = np.array([factor_brillo, factor_brillo, factor_brillo], dtype=np.uint8)
    img_brillo = np.zeros_like(img)
    
    # 2. Creación y lanzamiento de hilos
    filas_por_hilo = alto // num_hilos
    lista_hilos = []
    
    for j in range(num_hilos):
        inicio_fila = j * filas_por_hilo
        fin_fila = alto if (j == num_hilos - 1) else (j + 1) * filas_por_hilo
        
        hilo = threading.Thread(
            target=procesar_bloque_brillo,
            args=(img, inicio_fila, fin_fila, vector_brillo, img_brillo)
        )
        lista_hilos.append(hilo)
        hilo.start()
        
    # 3. Sincronización
    for hilo in lista_hilos:
        hilo.join()
        
    # 4. Escritura a disco
    nombre_salida = f"resultado_temp_{num_hilos}.bmp"
    cv2.imwrite(nombre_salida, img_brillo)
    
    t_fin = time.perf_counter()
    return (t_fin - t_inicio) * 1000  # Retorna en ms


if __name__ == "__main__":
    print("--- Benchmark de Extremo a Extremo (Reporte Completo de 100 Corridas) ---")
    ruta = seleccionar_imagen()
    
    if ruta:
        configuraciones_hilos = [1, 2, 4, 8, 16]
        datos_grafica = []
        num_repeticiones = 100
        
        for hilos in configuraciones_hilos:
            print(f"\n[PROCESANDO CONFIGURACIÓN: {hilos} HILO(S)]")
            tiempos_configuracion = []
            
            # Ejecutar el benchmark 100 veces para esta cantidad de hilos
            for corrida in range(num_repeticiones):
                tiempo_ms = ejecutar_corrida_completa(ruta, num_hilos=hilos)
                if tiempo_ms is not None:
                    tiempos_configuracion.append(tiempo_ms)
            
            # --- IMPRESIÓN DEL REPORTE DETALLADO DE LAS 100 CORRIDAS ---
            print("\n" + "="*45)
            print(f"{'Corrida':<15} | {'Tiempo de Ejecución (ms)':<25}")
            print(f"Configuración: {hilos} Hilos")
            print("="*45)
            
            for idx, t in enumerate(tiempos_configuracion):
                print(f"Corrida {idx+1:<8} | {t:.4f} ms")
                
            print("="*45)
            print(f"PROMEDIO        | {np.mean(tiempos_configuracion):.4f} ms")
            print(f"MÁS RÁPIDO      | {np.min(tiempos_configuracion):.4f} ms")
            print(f"MÁS LENTO       | {np.max(tiempos_configuracion):.4f} ms")
            print("="*45 + "\n")
            
            # Guardar datos para la gráfica final
            datos_grafica.append(tiempos_configuracion)
        
        # =========================================================================
        # GENERACIÓN DEL DIAGRAMA DE CAJAS Y BIGOTES (BOXPLOT)
        # =========================================================================
        print("Generando gráficos estadísticos...")
        plt.figure(figsize=(12, 7))
        
        box = plt.boxplot(datos_grafica, 
                          labels=[f"{h} Hilos" for h in configuraciones_hilos],
                          patch_artist=True,
                          flierprops=dict(marker='o', markerfacecolor='red', markersize=5, linestyle='none'))
        
        colores = ['#aec7e8', '#ffbb78', '#98df8a', '#ff9896', '#c5b0d5']
        for patch, color in zip(box['boxes'], colores):
            patch.set_facecolor(color)
            
        plt.title('Análisis de Rendimiento de Extremo a Extremo en Python\n(Incluye Lectura de Disco, Creación de Hilos y Escritura de Disco)', fontsize=12, fontweight='bold')
        plt.xlabel('Configuración de Hilos', fontsize=10)
        plt.ylabel('Tiempo Total de Ejecución (ms)', fontsize=10)
        plt.grid(True, linestyle=':', alpha=0.6, axis='y')
        
        print("Mostrando diagrama en pantalla.")
        plt.show()
        
    else:
        print("No se seleccionó ninguna imagen.")