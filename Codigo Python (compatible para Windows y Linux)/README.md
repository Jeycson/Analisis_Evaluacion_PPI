# Procesador de Brillo en Imágenes BMP con Python

## Descripción

Este proyecto permite seleccionar una imagen BMP de 24 bits y aumentar su brillo utilizando procesamiento paralelo mediante múltiples hilos (*threads*). El tiempo de ejecución de cada procesamiento se mide y se muestra en milisegundos.

---

## Requisitos

Antes de ejecutar el programa es necesario instalar:

* Python 3.10 o superior.
* Las bibliotecas:

  * OpenCV
  * NumPy
  * Matplotlib

> **Nota:** Tkinter normalmente ya viene incluido con Python.

---

## Instalación

### 1. Instalar Python

Descargar Python desde:

https://www.python.org/downloads/

Durante la instalación, asegurarse de marcar la opción:

```text
Add Python to PATH
```

---

### 2. Verificar la instalación

Abrir una terminal y ejecutar:

```bash
python --version
```

o:

```bash
python3 --version
```

Debería aparecer algo similar a:

```text
Python 3.13.2
```

---

### 3. Descargar el archivo

Guardar el archivo:

```text
procesar_brillo.py
```

en una carpeta de trabajo.

---

### 4. Instalar las dependencias

Abrir una terminal dentro de la carpeta del proyecto y ejecutar:

```bash
pip install opencv-python numpy matplotlib
```

Si el comando anterior no funciona:

```bash
pip3 install opencv-python numpy matplotlib
```

---

## Ejecución

Desde la terminal, ubicarse en la carpeta donde se encuentra el archivo y ejecutar:

```bash
python procesar_brillo.py
```

o:

```bash
python3 procesar_brillo.py
```

---

## Funcionamiento

1. Se abrirá una ventana para seleccionar una imagen BMP.
2. El programa dividirá la imagen en bloques.
3. Se crearán 16 hilos para procesar la imagen en paralelo.
4. Se aumentará el brillo de la imagen.
5. Se guardará una imagen llamada:

```text
resultado_temp_16.bmp
```

6. El proceso se repetirá 100 veces para obtener tiempos de ejecución.
7. Los tiempos (en milisegundos) se mostrarán en la consola.

---

## Estructura del proyecto

```text
Proyecto/
│
├── procesar_brillo.py
├── imagen.bmp
└── resultado_temp_16.bmp
```

---

## Dependencias utilizadas

| Biblioteca     | Uso                                               |
| -------------- | ------------------------------------------------- |
| `cv2` (OpenCV) | Lectura y escritura de imágenes                   |
| `numpy`        | Manipulación de arreglos y vectores               |
| `threading`    | Procesamiento concurrente                         |
| `time`         | Medición del tiempo de ejecución                  |
| `tkinter`      | Selección de archivos mediante interfaz gráfica   |
| `matplotlib`   | Generación de gráficas (actualmente no utilizada) |

---

## Ejemplo de salida

```text
13.51
12.83
14.27
13.06
12.95
...
```

Cada número representa el tiempo de ejecución de una corrida completa en milisegundos.

---

---

## Mensaje cuando no se selecciona una imagen

Si no se selecciona ninguna imagen, aparecerá:

```text
No se seleccionó ninguna imagen.
```

---
