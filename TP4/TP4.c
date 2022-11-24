/*
Trabajo Práctico N° 4
Materia: INF289 - Algoritmos Concurrentes y Paralelos
Alumo: MIELE, Ariel Gerardo
DNI: 34.434.704
Legajo: VINF011482
Año: 2022
*/

/**
 * CONSIGNAS DE LA ACTIVIDAD
 * Diseñar y desarrollar una aplicación que permita llevar a cabo una multiplicación de matrices en paralelo mediante MPI.
 * La forma de realizar la división de trabajo entre los procesos queda a criterio de cada estudiante.
 * Como requisito el tamaño de las matrices debe poder ser definido por el usuario, las matrices deben ser cuadradas.
 * Se deberá llevar a cabo una experimentación y posterior análisis de resultados con matrices de diferentes tamaños.
 * Es recomendable trabajar con reserva dinámica de memoria.
 * El proceso máster (o PID 0) inicializará las matrices con valores aleatorios de tipo doble y repartirá la carga de forma adecuada, según el criterio.
 * Se deberá registrar y mostrar por pantalla el tiempo empleado en la ejecución, y los resultados de salida serán escritos en un archivo en disco (NO REQUERIDO POR EL TP).
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

// Define el tamaño de la matriz, se trabaja con matrices cuadradas
#define n 3

void imprimirMatriz(int m[n][n], char nombre[])
{
    printf("Matriz %c: \n", nombre);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; i < n; j++)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

void generarMatriz(int m[n][n])
{
    // Inicializa las matrices con números aleatorios entre 0 y 9
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; i < n; j++)
        {
            m[i][j] = rand() % 10;
        }
    }
}

int main(int argc, char *argv[])
{
    // Se inicia la semilla para la generación de números aleatorios
    srand(time(0));

    int pid, np, elementos_por_proceso, n_elementos_recibidos;
    // np -> numero de procesos
    // pid -> id del proceso

    MPI_Status status;

    // Creación de procesos paralelos
    MPI_Init(&argc, &argv);

    // Encontrar el PID y procesos iniciados
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    // Proceso principal
    if (pid == 0)
    {
        // Declaración de variables
        int index, i, j;

        // Se deben generar las matrices A y B
        int matrizA[n][n];
        int matrizB[n][n];

        generarMatriz(matrizA);
        generarMatriz(matrizB);

        // Muestra las Matrices por pantalla
        imprimirMatriz(matrizA, "A");
        imprimirMatriz(matrizB, "B");

        // Distribuir las filas de la Matriz A en cada uno de los procesos
        MPI_Scatter(&matrizA, 1, MPI_INT, &pid, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Enviar la Matriz B a todos los procesos
        MPI_Bcast(&matrizB, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Obtener sumas parciales de otros procesos
        int temp[n][n];
        MPI_Gather(&temp, 1, MPI_INT, &pid, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Proceso principal reconstruye la matriz solución
        int solucion[n][n];

        // Imprimir la suma final del arreglo
        printf("Suma total del arreglo es: %d\n", sum);
    }
    // Procesos hijos
    else
    {
        MPI_Recv(&n_elements_retrieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        // Guardar los segmentos de arreglos recibidos en un arreglo local a2
        MPI_Recv(&a2, n_elements_retrieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        // Calcular su suma parcial
        int partial_sum = 0;
        for (int i = 0; i < n_elements_retrieved; i++)
            partial_sum += a2[i];
        printf("Suma parcial del arreglo es: %d\n", partial_sum);
        // Enviar la suma parcial al proceso principal o razí
        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    // Limpiar todos los estados de MPI antes de salir de proceso
    MPI_Finalize();

    return 0;
}
