/*
Trabajo Práctico N° 2
Materia: INF289 - Algoritmos Concurrentes y Paralelos
Alumo: MIELE, Ariel Gerardo
DNI: 34.434.704
Legajo: VINF011482
Año: 2022
*/

/*
CONSIGNA A

Implementa un algoritmo en C que reciba como argumento dos números enteros, N y M. Tales números serán utilizados para crear N hilos (threads) de tipo 1 y M hilos (threads) de tipo 2. A continuación se describe la funcionalidad que deberás implementar cada uno de los hilos:

Hilo de tipo 1: Deberás mostrar por pantalla el mensaje: “Instancia x del hilo 1”, siendo X el número de hilo creado (entre 0 y N-1). Posteriormente, se suspenderá por un tiempo aleatorio entre 0 y 2 segundos (para ello utilizarás la llamada usleep o sleep). Finalmente, incrementará la variable global “Compartida”.

Hilo de tipo 2: Deberás mostrar por pantalla el siguiente mensaje: “Instancia y del hilo 2”, siendo Y el número de hilo creado (entre 0 y M-1). Posteriormente, se suspenderá por un tiempo aleatorio entre 0 y 2 segundos. Para finalizar, leerá y mostrará por pantalla el valor de la variable global “Compartida”.

Cuando los hilos finalicen su ejecución, el padre debe mostrar por pantalla “Se ha finalizado la ejecución”. Ver funcionamiento de pthread_create y pthread_join.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

// Se define la variable global compartida, inicializada en 0
int compartida = 0;

// Tarea que realizará el Hilo de Tipo 1
void *tareaHiloN(int instancia)
{
    // Se muestra por pantalla la isntancia del hilo y su ID
    printf("Instancia %d del hilo de Tipo 1.\n", instancia);
    // Se suspende por un tiempo aleatorio entre 0 y 2 segundos el hilo

    sleep(demoraAleatoria());
    // Se incrementa en 1 la variable global "compartida"
    compartida++;
}

// Tarea que realizará el Hilo del Tipo 2
void *tareaHiloM(int instancia)
{
    // Se muestra por pantalla la isntancia del hilo y su ID
    printf("Instancia %d del hilo de Tipo 2.\n", instancia);
    // Se suspende por un tiempo aleatorio entre 0 y 2 segundos el hilo
    sleep(demoraAleatoria());
    // Se muestra por pantalla la veriable gobal "compartida"
    printf("Valor de la variable global 'compartida': %d\n", compartida);
}

int demoraAleatoria()
{
    // La demora aleatoria se definirá entre 0 y 2 segundos

    // Se define la variable para almacenar la demora aleatoria, una variable para el valor mínimo y una para el valor máximo
    int dAleatoria, minD = 0, maxD = 2;

    // Retorna el número de demora aleatoria
    return dAleatoria = minD + rand() % (maxD - minD + 1);
}

int main(int argc, char const *argv[])
{
    // Se inicia la semilla para la generación de números aleatorios
    srand(time(0));

    // Se crean las variables a utilizar para la llamada de la función
    int N, M;

    // Se solicita al usuario ingresar los números de hilos a crear
    printf("Seleccione la cantidad de hilos de Tipo 1 a crear (N): ");
    // Se almacena el numero en la variable N
    scanf("%d", &N);

    // Hacemos lo mismo para la variable M
    printf("Seleccione la cantidad de hilos de Tipo 2 a crear (M): ");
    scanf("%d", &M);

    printf("\nSe crean %d hilos del Tipo 1 y %d hilos del Tipo 2.\n\n", N, M);

    // Se crea el arreglo de hilos N
    pthread_t thN[N];

    // Se crea el arreglo de hilos M
    pthread_t thM[M];

    // Creación de los hilos N de Tipo 1
    for (int i = 0; i < N; i++)
    {
        // Se crea el hilo dentro de su posición en el arreglo de hilos, se le asigna la tarea a realizar, pasándole el valor i como identificador de la instancia
        pthread_create(&thN[i], NULL, &tareaHiloN, i);
    }

    // Join de los hilos N de Tipo 1
    for (int i = 0; i < N; i++)
    {
        pthread_join(thN[i], NULL);
    }

    // Creación de los hilos M de Tipo 2
    for (int i = 0; i < M; i++)
    {
        // Se crea el hilo dentro de su posición en el arreglo de hilos, se le asigna la tarea a realizar, pasándole el valor i como identificador de la instancia
        pthread_create(&thM[i], NULL, &tareaHiloM, i);
    }

    // Join de los hilos M de Tipo 2
    for (int i = 0; i < M; i++)
    {
        pthread_join(thM[i], NULL);
    }

    // Se indica al usuario que la ejecución a finalizado
    printf("La ejecucion ha finalizado.\n");

    return 0;
}