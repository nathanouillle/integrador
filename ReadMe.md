# Sistema de detección de cascos con Raspberry Pi

## Descripción del proyecto
El objetivo es utilizar una Raspberry Pi, una cámara y un LED para detectar automáticamente si alguien está usando un casco de seguridad en una foto capturada. Esto se logrará mediante la integración de la IA existente desarrollada en Python con la Raspberry Pi, donde la captura de la imagen y el control del LED se implementarán en C, mientras que la IA se mantendrá en Python.

## Pasos para el desarrollo del proyecto

### Configuración del hardware

* Conecta una cámara compatible con la Raspberry Pi al puerto correspondiente.
* Conecta un LED al pin GPIO de la Raspberry Pi. Asegúrate de utilizar un resistor apropiado para limitar la corriente del LED.

### Desarrollo del código en C

* Escribe un programa en C utilizando las bibliotecas adecuadas para la Raspberry Pi, que capture una imagen utilizando la cámara y la guarde en la memoria de la Raspberry Pi.
* Implementa la lógica para controlar el LED en C, utilizando los pines GPIO de la Raspberry Pi.

### Comunicación con la IA en Python

* Desde el código en C, llama a un script de Python utilizando la biblioteca adecuada para enviar la imagen capturada al modelo de IA existente desarrollado para la detección de cascos.
* Configura la comunicación entre el código en C y Python utilizando mecanismos como pipes, sockets, archivos compartidos, etc.

### Procesamiento de la imagen con Python

* En el script de Python, implementa la lógica para procesar la imagen recibida y ejecutar la IA para la detección de cascos.
* Devuelve el resultado de la IA (si se detecta un casco o no) al código en C.

### Control del LED en C

Utilizando los resultados recibidos desde Python, implementa la lógica en el código en C para encender o apagar el LED según corresponda.

### Pruebas y refinamiento

Ejecuta el programa en la Raspberry Pi y prueba la funcionalidad.
Realiza pruebas con diferentes imágenes que contengan personas con y sin cascos para validar la precisión del sistema.


## Comunicación C a Python

1. Serialización de la imagen en C

* Después de capturar la imagen en el código en C, serializarla en un formato que se pueda transmitir a través de la comunicación con Python.
* Se puede convertir la imagen en una matriz de bytes o guardarla en un archivo temporal en disco utilizando una biblioteca como OpenCV.
* Asegúrase de que la imagen esté codificada y formateada correctamente para la transmisión.

Pensamos usar una *matriz de bytes* para un uso liviano de los recursos.

2. Comunicación entre C y Python

Existen varias opciones para establecer la comunicación entre el código en C y Python. Algunas de ellas son:

* Uso de pipes: Se puede crear un pipe para establecer una conexión entre los procesos de C y Python. El código en C puede escribir los datos de la imagen en el pipe, y el código en Python puede leerlos desde el pipe.
* Uso de sockets: Se puede crear un socket para la comunicación entre C y Python. El código en C puede enviar los datos de la imagen a través del socket, y el código en Python puede recibirlos desde el socket.
* Uso de archivos compartidos: Se puede guardar la imagen en un archivo temporal en C y luego acceder a ese archivo desde el código en Python.

Pensamos usar *pipes* para mas eficiencia y rapidez.

3. Recepción y procesamiento de la imagen en Python

En el código de Python, recibe los datos de la imagen enviados por el código en C a través de la comunicación establecida.
Si se utiliza una matriz de bytes, convertirla nuevamente en una imagen utilizando bibliotecas como OpenCV o PIL.
Si se utiliza un archivo temporal en disco, cargar la imagen desde el archivo utilizando las bibliotecas mencionadas anteriormente.

Pensamos usar una matriz de bytes, por los tanto usar OpenCV

4. Ejemplo
Lado C
``` c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2]; // Descriptores de archivo para el pipe
    pipe(fd); // Crear el pipe

    // Se supone que hay una matriz de bytes llamada 'imageData' con su contenido

    // Escribir la matriz de bytes en el pipe
    write(fd[1], imageData, imageSize);

    close(fd[1]); // Cerrar el extremo de escritura del pipe

    return 0;
}
```
Lado Python
``` python
import os
import numpy as np
import cv2

# Leer los datos de la matriz de bytes desde el pipe
read_fd = os.fdopen(os.dup(fd[0]), 'rb')
image_data = read_fd.read()
read_fd.close()

# Convertir la matriz de bytes en una imagen utilizando OpenCV
nparr = np.frombuffer(image_data, np.uint8)
image = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

# Continuar con la ejecución de la IA
```