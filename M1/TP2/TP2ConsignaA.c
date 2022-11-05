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
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    // getpid() devuelve el identificador del proceso. Podríamos usar getppid() para devolver el identificador del proceso padre.

    // Creamos el proceso hijo del proceso principal; almacenamos el identificador del proceso en una variable int

    int pid = fork();

    // Aquí se ejecutarán instrucciones tanto del padre (pid != 0) como del hijo (pid == 0)

    if (pid != 0)
    {
        // Estas instrucciones las ejecutará sólo el proceso padre
        printf("Soy el proceso padre, mi id es: %d\n", getpid());
    }
    else
    {
        // Estas instrucciones las ejecutará sólo el proceso hijo
        printf("Soy el proceso hijo, mi id es: %d\n", getpid());
    }

    return 0;
}