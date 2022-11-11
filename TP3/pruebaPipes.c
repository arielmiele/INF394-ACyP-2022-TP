/**
 * Un programa con un proceso padre y un proceso hijo.
 * El hijo va a escribir un número entero, el padre va a leer el entero y lo multiplicará por 3.
 * Se usa la llamada fork() para crear el proceso hijo.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int fd[2]; // File descriptor para lectura fd[0] y escritura fd[1]

    if (pipe(fd) == -1)
    { // Abrir pipe y manejo de error
        printf("Ha ocurrido un error al abrir el pipe.\n");
        return 1; // Retorno por si hay algún error
    }

    int id = fork(); // Llamada fork crear proceso

    if (id == -1)
    { // Manejo de error en fork
        printf("Ha ocurrido un error con fork.\n");
        return 2;
    }

    if (id == 0)
    {                 // Proceso hijo
        close(fd[0]); // Cierro el file descriptor de lectura, no se va a leer en este proceso hijo
        int x;
        printf("Ingrese un numero: ");
        scanf("%d", &x);
        write(fd[1], &x, sizeof(int)); // Escribir en el file descriptor
        close(fd[1]);                  // Cierro el file descriptor de escritura
    }
    else
    { // Proceso padre
        int y;
        read(fd[0], &y, sizeof(int));                         // Leo el file descritpor de lectura
        close(fd[0]);                                         // Cierro el file descriptor de lectura
        printf("Dato obtenido del proceso hijo %d\n", y * 3); // Salida en pantalla
    }

    return 0;
}
