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
    img = cv2.imread(ruta_imagen)
    if img is None:
        return None
        
    alto, ancho, _ = img.shape
    vector_brillo = np.array([factor_brillo, factor_brillo, factor_brillo], dtype=np.uint8)
    img_brillo = np.zeros_like(img)
    
    t_inicio = time.perf_counter()
    
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
        
    
    t_fin = time.perf_counter()
    # 4. Escritura a disco
    nombre_salida = f"resultado_temp_{num_hilos}.bmp"
    cv2.imwrite(nombre_salida, img_brillo)
    
    return (t_fin - t_inicio) * 1000  # Retorna en ms


if __name__ == "__main__":
    ruta = seleccionar_imagen()
    
    if ruta:
        datos_grafica = []
        num_repeticiones = 100
        
        tiempos_configuracion = []
        
        for corrida in range(num_repeticiones):
            tiempo_ms = ejecutar_corrida_completa(ruta, num_hilos=1)
            if tiempo_ms is not None:
                tiempos_configuracion.append(tiempo_ms)
        
        for idx, t in enumerate(tiempos_configuracion):
            print(t)
        
    else:
        print("No se seleccionó ninguna imagen.")