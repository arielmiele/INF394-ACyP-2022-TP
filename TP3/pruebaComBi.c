/**
 * Se analiza un algoritmo en el que se crean dos procesos, que podemos llamar P1 y P2.
 * P1 toma un string y se lo envía a P2. El proceso P2 concatena el string recibido con otra cadena (sin usar la función de string)
 * y la envía de regreso al proceso P1 para que se imprima en pantalla. Para este algoritmo se usarán dos pipes,
 * y por ende dos arreglos de files descriptors, y se usará fork() para crear el proceso hijo.
 *
 * El algoritmo busca concetenar cadenas de la siguiente manera:
 *
 * Entrada: www | Otra Cadena: google.com | Salida: www.google.com
 * Entrada: www.argentina | Otra cadena: .gob.ar | Salida: www.argentina.gob.ar
 *
 * Se usarán dos pipes, en el primero se envía un string de entrada desde el proceso padre al hijo.
 * En el segundo pipe se envía un string concatenado desde el proceso hijo al padre.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int fd1[2];                   // Usado para almacenar dos extremos del primer pipe
    int fd2[2];                   // Usado para almacenar dos extremos del segundo pipe
    char fixed_str[] = ".gob.ar"; // String fijo
    char input_str[100];          // String de entrada
    pid_t p;

    // Manejo de error
    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Fallo de Pipe");
        return 1;
    }

    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "fallo de Pipe");
        return 1;
    }

    printf("Ingrese cadena a concatenar: ");
    scanf("%s", input_str); // Espera ingreso de cadena

    p = fork(); // Llamada fork()

    // Manejo de error fork
    if (p < 0)
    {
        fprintf(stderr, "Falla de Fork");
        return 1;
    }

    // Proceso padre
    else if (p > 0)
    {
        char concat_str[100];

        close(fd1[0]); // Cierra la lectura del primer pipe

        // Escribe string de entrada y cierra escritura del primer pipe
        write(fd1[1], input_str, strlen(input_str) + 1);
        close(fd1[1]);

        // Espera que el proceso hijo envíe una cadena

        wait(NULL);

        close(fd2[1]); // Cierra escritura del segundo pipe

        // Lee string del proceso hijo, imprime y cierra lectura del segundo pipe
        read(fd2[0], concat_str, 100);
        printf("Cadena contactenada %s\n", concat_str);
        close(fd2[0]); // Cierre lectura segundo pipe
    }

    // Proceso hijo
    else
    {
        close(fd1[1]); // Cierra escritura del primer pipe

        // Lee un string usando el primer pipe
        char concat_str[100];
        read(fd1[0], concat_str, 100);

        // Concatena una cadena fija
        int k = strlen(concat_str);
        int i;
        for (i = 0; i < strlen(fixed_str); i++)
            concat_str[k++] = fixed_str[i];

        concat_str[k] = '\0'; // String termina con '\0'

        // Cierra ambos finales de lectura
        close(fd1[0]);
        close(fd2[0]);

        // Escribe string concatenado y cierra final de escritura
        write(fd2[1], concat_str, strlen(concat_str) + 1);
        close(fd2[1]);

        exit(0);
    }
}
