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