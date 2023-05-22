#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <curl/curl.h>

#define URL_API "http://localhost:5000/api/image"

// Estructura para almacenar los datos necesarios para enviar la imagen a la API
struct ImageData
{
    cv::Mat image;
    bool success;
};

void *capturarImagen(void *threadid)
{
    cv::VideoCapture cap(0); // Abrir la cámara (puedes cambiar el índice si tienes varias cámaras conectadas)

    if (!cap.isOpened())
    {
        printf("Error al abrir la cámara\n");
        pthread_exit(NULL);
    }

    cv::Mat frame; // Objeto para almacenar el fotograma capturado

    while (1)
    {
        cap.read(frame); // Capturar un fotograma de la cámara

        // Verificar si se ha capturado correctamente el fotograma
        if (frame.empty())
        {
            printf("Error al capturar imagen\n");
            break;
        }

        // Enviar la imagen capturada a la API en un hilo separado
        struct ImageData *imageData = (struct ImageData *)malloc(sizeof(struct ImageData));
        imageData->image = frame.clone();
        imageData->success = false;

        pthread_t thread;
        int api;

        printf("Creando hilo de envío de imagen\n");
        api = pthread_create(&thread, NULL, enviarImagenAPI, (void *)imageData);

        if (api)
        {
            printf("Error al crear el hilo de envío de imagen\n");
            free(imageData);
            continue;
        }

        // Esperar a que el hilo de envío de imagen termine
        pthread_join(thread, NULL);

        if (imageData->success)
        {
            printf("Imagen enviada exitosamente a la API\n");
        }
        else
        {
            printf("Error al enviar la imagen a la API\n");
        }

        free(imageData);

        // Simulación de captura periódica de imagen
        struct timespec tim;
        tim.tv_sec = 1;
        tim.tv_nsec = 0;
        nanosleep(&tim, NULL);
    }

    cap.release(); // Liberar los recursos de la cámara
    pthread_exit(NULL);
}

void *enviarImagenAPI(void *imageData)
{
    struct ImageData *data = (struct ImageData *)imageData;

    // Preparar la imagen para enviarla a la API (si es necesario)

    // Crear un objeto cURL
    CURL *curl;
    CURLcode response;

    // Inicializar cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        // Configurar la URL de la API
        curl_easy_setopt(curl, CURLOPT_URL, URL_API);

        // Establecer el tipo de contenido y los datos de la imagen
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Content-Type: text/plain");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data->image.data); //.data es un puntero a los datos de la imagen
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data->image.total() * data->image.elemSize());
        // .total() devuelve el número de pixeles de la matriz
        // .elemSize() devuelve el tamaño de un pixel -> 3 bytes (RGB)

        // Realizar la solicitud HTTP POST a la API
        response = curl_easy_perform(curl);

        // Verificar si la solicitud se realizó correctamente
        if (response == CURLE_OK)
        {
            data->success = true;
        }

        // Limpiar y liberar los recursos de cURL
        curl_easy_cleanup(curl);
    }

    // Finalizar cURL
    curl_global_cleanup();

    pthread_exit(NULL);
}

int main()
{
    pthread_t thread;
    int capture;

    printf("Creando hilo de captura\n");
    capture = pthread_create(&thread, NULL, capturarImagen, NULL);

    if (capture)
    {
        printf("Error al crear el hilo de captura\n");
        exit(-1);
    }

    // Espera a que el hilo de captura termine
    pthread_join(thread, NULL);

    printf("Hilo de captura ha terminado. Programa finalizado.\n");
    pthread_exit(NULL);
}
