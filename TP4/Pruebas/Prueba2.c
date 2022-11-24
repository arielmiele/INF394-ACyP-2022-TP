/**
 * Esta prueba suma los elementos de un arreglo.
 * Puede servir para reducir la complejidad en el tiempo de ejecución del programa.
 * Se ejecutan paralelamente subarreglos, se calculan sumas parciales.
 * Finalmente el proceso maestro (raíz) calcula la suma de estas sumas parciales y devuelve la suma total del arreglo.
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Tamaño del arreglo
#define n 10

int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// Arreglo temporal para subprocesos
int a2[1000];

int main(int argc, char *argv[])
{
    int pid, np, elements_per_process, n_elements_retrieved;
    // np -> numero de procesos
    // pid -> id del proceso

    MPI_Status status;

    // Creación de procesos paralelos
    MPI_Init(&argc, &argv);

    // Encontrar el PID y como muchos procesos fueron iniciados
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    // Proceso principal
    if (pid == 0)
    {
        int index, i;
        elements_per_process = n / np;

        // Verificar si se están ejecutando más de un proceso
        if (np > 1)
        {
            // Distribuir la porción del arreglo a procesos hijos para calcular su suma parcial
            for (i = 1; i < np - 1; i++)
            {
                index = i * elements_per_process;
                MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&a[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            // El ultimo proceso agrega los elementos remanentes
            index = i * elements_per_process;
            int elements_left = n - index;
            MPI_Send(&elements_left, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // Proceso principal agrega su propio sub arreglo
        int sum = 0;
        for (i = 0; i < elements_per_process; i++)
        {
            sum += a[i];
        }
        printf("Suma parcial del proceso principal: %d\n", sum);
        // Obtener sumas parciales de otros procesos
        int tmp;
        for (i = 1; i < np; i++)
        {
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            int sender = status.MPI_SOURCE;
            sum += tmp;
        }
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