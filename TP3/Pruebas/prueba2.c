/**
 * Definimos un arreglo de enteros, y vamos a sumar sus elementos.
 * Se va a dividir el arreglo en dos partes (a la mitad), una mitad de los elementos será trabajada por uno de los procesos,
 * y la otra mitad de elementos por el otro proceso.
 * Ambos procesos obtendrán una suma parcial, uno de los procesos escribirá su suma parcial en el pipe,
 * y el otro proceso leerá esa suma parcial del pipe y obtendrá la suma total para mostrarla en pantalla.
 * Para efectuar las sumas parciales, cada proceso debe saber por cual elemento del arreglo debe empezar a sumar,
 * y por cual elemento debe terminar su suma parcial.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int arr[] = {4, 5, 7, 3, 1, 8};
    int inicio, final;                       // Indices para sumar parciales
    int longArr = sizeof(arr) / sizeof(int); // Cantidad elementos
    int fd[2];                               // File descriptor

    // Abre pipe y manejo de error
    if (pipe(fd) == -1)
    {
        printf("Ha ocurrido un error al abrir el pipe.\n");
        return 1;
    }

    // Crear proceso
    int id = fork();

    // Manejo de error en fork
    if (id == -1)
    {
        printf("Ha ocurrido un error con fork.\n");
        return 2;
    }

    // Proceso hijo
    if (id == 0)
    {
        inicio = 0;
        final = longArr / 2;
    }
    else
    {
        // Proceso padre
        inicio = longArr / 2;
        final = longArr;
    }

    int suma = 0;
    int i;
    for (i = inicio; i < final; i++)
    {
        suma += arr[i];
    }

    printf("Suma parcial calculada: %d\n", suma);

    return 0;
}
