/*
Trabajo Práctico N° 2
Materia: INF289 - Algoritmos Concurrentes y Paralelos
Alumo: MIELE, Ariel Gerardo
DNI: 34.434.704
Legajo: VINF011482
Año: 2022
*/

/*
CONSIGNA B

Continúa con lo realizado en la consigna anterior, pero esta vez utilizando procesos en vez de hilos. Para ello, deberás crear, por medio de la llamada fork, N procesos Tipo 1 y M procesos de Tipo 2. Realiza un análisis del comportamiento de ambas implementaciones y detalla el comportamiento observado, diferencias, describir.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// Se define la variable global compartida, inicializada en 0
int compartida = 0;

// Tarea que realizará el Proceso de Tipo 1
// instP1 = numero de la instancia del proceso
void tareaProceso1(int instP1)
{
    // Se muestra por pantalla la instancia del Proceso
    printf("Instancia %d del proceso de Tipo 1.\n", instP1);
    // Se incrementa en 1 la variable global "compartida"
    compartida++;
    printf("Se incrementa la variable compartida, nuevo valor:: %d\n", compartida);
}

// Tarea que realizará el Proceso del Tipo 2
// instP1 = numero de la instancia del proceso
void tareaProceso2(int instP2)
{
    // Se muestra por pantalla la instancia del Proceso
    printf("Instancia %d del proceso de Tipo 2.\n", instP2);
    // Se muestra por pantalla la veriable gobal "compartida"
    printf("Valor de la variable global 'compartida': %d\n", compartida);
}

int main(int argc, char const *argv[])
{
    // Se crean las variables a utilizar para la llamada de la función
    // procNum1 y procNum2 almacenan los numeros de procesos del Tipo 1 y Tipo 2, respectivamente
    int N, M, procNum1, procNum2;

    // Se crea una variable para identificar los ID de los procesos
    pid_t pid1, pid2;

    // Se solicita al usuario ingresar los números de procesos a crear
    printf("Seleccione la cantidad de procesos de Tipo 1 a crear (N): ");
    // Se almacena el numero en la variable N
    scanf("%d", &N);

    // Hacemos lo mismo para la variable M
    printf("Seleccione la cantidad de procesos de Tipo 2 a crear (M): ");
    scanf("%d", &M);

    printf("\nSe crean %d procesos del Tipo 1 y %d procesos del Tipo 2.\n\n", N, M);

    // Procesos de Tipo 1
    for (procNum1 = 0; procNum1 < N; procNum1++)
    {
        // Se crea el proceso hijo
        pid1 = fork();
        if (pid1 == 0)
        {
            // Proceso creado correctamente, se sale del for
            break;
        }
        else if (pid1 == -1)
        {
            // Se finaliza el ciclo for por error al crear el proceso
            perror("ERROR al llamar fork, no se puede crear el proceso.\n");
            exit(1);
            break;
        }
    }

    if (pid1 == 0)
    {
        // Ejecución del proceso Hijo de Tipo 1
        tareaProceso1(procNum1);
        exit(0);
    }
    else
    {
        // Ejecución del proceso Padre - Espera que todos los procesos hijos terminen
        for (int i = 0; i < N; i++)
        {
            if ((pid1 = wait(NULL)) >= 0)
            {
                // Proceso hijo finalizado
            }
        }
        // Luego de haber finalizado todos los procesos hijos, el proceso padre finaliza
        printf("La ejecucion de los procesos de Tipo 1 ha finalizado.\n");
    }

    // Procesos de Tipo 2
    for (procNum2 = 0; procNum2 < M; procNum2++)
    {
        // Se crea el proceso hijo
        pid2 = fork();
        if (pid2 == 0)
        {
            // Proceso creado correctamente, se sale del for}
            break;
        }
        else if (pid2 == -1)
        {
            // Se finaliza el ciclo for por error al crear el proceso
            perror("ERROR al llamar fork, no se puede crear el proceso.\n");
            exit(1);
            break;
        }
    }

    if (pid2 == 0)
    {
        // Ejecución del proceso Hijo de Tipo 1
        tareaProceso2(procNum2);
        exit(0);
    }
    else
    {
        // Ejecución del proceso Padre - Espera que todos los procesos hijos terminen
        for (int i = 0; i < M; i++)
        {
            if ((pid2 = wait(NULL)) >= 0)
            {
                // Proceso hijo finalizado
            }
        }
        // Luego de haber finalizado todos los procesos hijos, el proceso padre finaliza
        printf("La ejecucion de los procesos de Tipo 2 ha finalizado.\n");
    }

    // Fin del programa
    return 0;
}