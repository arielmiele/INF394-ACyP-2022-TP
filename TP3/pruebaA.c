#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

/**
 * Inicializa el mazo
 */
void incializar_mazo(float mazo[4][10]);

// Devuelve un valor al azar de entre las cartas que no se han sacado
// A partir del arreglo de la baraja
float saca_carta(float mazo[4][10]);

// Muestra el tamaño del mazo
int tamanio_mazo(float mazo[4][10]);

// Imprime el mazo
void imprimir_mazo(float mazo[4][10]);

const int SIZE_OF_FLOAT = sizeof(float);
const int SIZE_OF_INT = sizeof(int);

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Inicializa la semilla para las aleatorizaciones

    int numJugadores;

    system("clear");
    printf("¡ Bienvenido a Siete y Medio ! Esperamos que se divierta.\n\nElija la cantidad de jugadores: ");
    scanf("%d", &numJugadores);

    printf("\nEl numero de jugadores es: %d.\n", numJugadores);

    float mazo[4][10];     // Definición del mazo
    incializar_mazo(mazo); // Inicialización del mazo
    imprimir_mazo(mazo);

    int numPipes = numJugadores * 2; // Calcula el número de pipes que deberán usarse
    int pids[numJugadores];          // Almacena los pid de los jugadores

    // Creación de los file descriptors para cada uno de los pipes
    // Los file descriptors de los pipes se asignarán de a pares para las conexiones con cada uno de los jugadores (ida y vuelta)
    int rep_a_jug[numJugadores][2]; // Pipe para comunicación de repartidor a jugador
    int i;
    for (i = 0; i < numJugadores + 1; i++)
    {
        if (pipe(rep_a_jug[i]) < 0)
        {
            printf("\nProblema abriendos los pipes de Repartidor a Jugador.\n");
            return 1;
        }
    }

    int jug_a_rep[numJugadores][2]; // Pipe para la comunicación de jugador a repartidor
    for (i = 0; i < numJugadores + 1; i++)
    {
        if (pipe(jug_a_rep[i]) < 0)
        {
            printf("\nProblema abriendo los pipes de Jugador a Repartidor.\n");
            return 2;
        }
    }

    // Inicio de creación de procesos
    for (i = 0; i < numJugadores; i++)
    {
        // Almacenamiento del ID del proceso creado en el array pid[]
        pids[i] = fork(); // Creación del proceso
        if (pids[i] == -1)
        {
            printf("\nERROR. No se pudo crear el proceso.\n");
            return 3;
        }
        if (pids[i] == 0)
        {
            // Proceso hijo
            printf("\nEl Jugador %d (ID: %d) ha entrado al juego.\n", i + 1, getpid());
            int j;
            // Cierre de los pipes que no serán usados por el proceso
            for (j = 0; j < numJugadores + 1; j++)
            {
                if (i != j)
                {
                    close(rep_a_jug[j][0]);
                    close(rep_a_jug[j][1]);
                    close(jug_a_rep[j][0]);
                    close(jug_a_rep[j][1]);
                }
            }

            // Proceso perteneciente al hijo
            float puntajeJ = 0.0; // Inicializa el puntaje del jugador en 0.0
            float puntajeC;       // Inicializa el puntaje de la carta entregada por el repartidor
            printf("\nSoy el Jugador %d (ID: %d), mi puntaje actual es: %.2f.\n", i + 1, getpid(), puntajeJ);

            wait(2);
            // Lee el puntaje de la carta enviada desde el repartidor
            if (read(rep_a_jug[i][0], &puntajeC, SIZE_OF_FLOAT) == -1)
            {
                printf("\nError en la lectura del pipe.\n");
                return 4;
            }

            // Mensaje del Jugador para mostrar qué carta recibió
            printf("\nSoy el Jugador %d (ID: %d), recibi una carta de puntaje: %.2f.\n", i + 1, getpid(), puntajeC);

            // Cálculo del nuevo puntaje del jugador
            puntajeJ += puntajeC;

            printf("\nSoy el Jugador %d (ID: %d), mi puntaje actual es: %.2f.\n", i + 1, getpid(), puntajeJ);

            // Envía el puntaje al repartidor
            if (write(jug_a_rep[i][1], &puntajeJ, SIZE_OF_FLOAT) == -1)
            {
                printf("\nError en la escritura del pipe.\n");
                return 5;
            }

            // Mensaje de salida del jugador
            printf("\nEl Jugador %d (ID: %d) ha finalizado su juego.\n", i + 1, getpid());
            // Cierre de los pipes abiertos
            close(rep_a_jug[i][0]);
            close(jug_a_rep[i][1]);
            return 0; // Finaliza la creación de procesos hijos (jugadores)
        }
    }

    // Proceso padre

    printf("\nEl Repartidor (ID: %d) ha entrado al juego.\n", getpid());

    float puntajes[numJugadores]; // Arreglo para almacenar los puntajes de los jugadores
    float puntosCarta = 0.0;      // Inicializa los puntos de la carta a enviar
    float puntajeJugador = 0.0;   // Inicializa el puntaje del jugador

    // Envía la carta al proceso
    for (int i = 0; numJugadores - 1; i++)
    {
        // Saca una carta del mazo
        puntosCarta = saca_carta(mazo);

        // Envía la carta por el pipe
        if (write(rep_a_jug[i][1], &puntosCarta, SIZE_OF_FLOAT) == -1)
        {
            printf("\nError en la escritura del pipe.\n");
            return 6;
        }

        printf("\nSoy el repartidor. Acabo de enviar una carta de puntaje %.2f al Jugador %d.\n", puntosCarta, i + 1);

        waitpid(pids[i], NULL, 0);

        // Lee el nuevo puntaje del jugador
        if (read(jug_a_rep[i][0], &puntajeJugador, SIZE_OF_FLOAT) == -1)
        {
            printf("\nError en la lectura del pipe.\n");
            return 7;
        }

        printf("\nSoy el repartidor. Acabo de leer el puntaje del Jugador %d. Puntaje: %.2f.\n", i + 1, puntajeJugador);

        // Agrega los puntajes al array de puntajes
        puntajes[i] = puntajeJugador;
    }

    // Espera que todos los jugadores finalicen sus turnos
    for (i = 0; i < numJugadores; i++)
    {
        wait(NULL);
    }

    // Se muestran los procesos
    for (i = 0; i < numJugadores; i++)
    {
        printf("\nProceso [%d] - Jugador [%d]: %d - Puntaje Final: %.2f\n", i, i + 1, pids[i], puntajes[i]);
    }

    return 0;
}

/**
 * Inicializa el mazo con la siguiente estructura (filas):
 *
 * 1 = ORO
 * 2 = COPA
 * 3 = BASTO
 * 4 = ESPADA
 *
 * El mazo no tiene las cartas 8 y 9
 * Si la posición contiene el valor 0.0, esa carta no está en el mazo
 */
void incializar_mazo(float mazo[4][10])
{

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (j <= 6)
            {
                mazo[i][j] = (float)j + 1.0; // Asigna valor 1.0 a 7.0 a las cartas del 1 al 7
            }
            else
            {
                mazo[i][j] = 0.5; // Asigna valor 0.5 a las cartas con figuras
            }
        }
    }
    printf("¡MAZO LISTO! Comienza el juego...\n\n");
    return;
}

/**
 * Muestra el tamaño del mazo
 * @return tamaño del mazo como entero (int)
 */
int tamanio_mazo(float mazo[4][10])
{
    int tamanio = 0;            // Definición de variable que almacenará el tamaño actual del mazo
    for (int i = 0; i < 4; i++) // Recorre las filas
    {
        for (int j = 0; j < 10; j++) // Recorre las columnas
        {
            if (mazo[i][j] != 0.0) // Contenido diferente de 0.0 equivale a carta disponible
            {
                tamanio++; // Incrementa en 1 el tamaño del mazo
            }
        }
    }
    return tamanio; // Devuelve el tamaño del mazo
}

/**
 * Imprime el mazo en la pantalla, usado para el análisis de la ejecución del juego
 */
void imprimir_mazo(float mazo[4][10])
{
    // Imprime el encabezado
    printf("\t1\t2\t3\t4\t5\t6\t7\tC\tS\tR\n");
    for (int i = 0; i < 4; i++) // Recorre las filas del mazo
    {
        // Imprime los palos en la primer columna
        if (i == 0)
        {
            printf("ORO\t");
        }
        else if (i == 1)
        {
            printf("COPA\t");
        }
        else if (i == 2)
        {
            printf("BASTO\t");
        }
        else
        {
            printf("ESPADA\t");
        }

        // Imprime los valores de las cartas
        for (int j = 0; j < 10; j++) // Recorre las columnas del mazo
        {
            printf("%.1f\t", mazo[i][j]); // Imprime por pantalla el valor de la posición (i;j) del array MAZO
        }
        printf("\n");
    }
}

/**
 * Extrae una carta del mazo, siempre que la carta se encuentre disponible
 * @return valor de la carta (float)
 */
float saca_carta(float mazo[4][10])
{
    // Inicializa variables
    float carta = 0.0;                    // Almacena el valor de la carta extraida
    int palo_aleatorio = rand() % 4;      // Calcula el palo de la carta (de 1 a 4), asociado a las filas de la matriz MAZO
    int posicion_aleatoria = rand() % 10; // Calcula la carta de forma aleatoria (de 1 a 10), asociado a las columnas de la matriz MAZO
    char palo[10];                        // Almacena el dato del palo para mostrar por pantalla

    // Extrae una carta del mazo hasta encontrar una carta disponible (distinto de 0.0)
    while (mazo[palo_aleatorio][posicion_aleatoria] == 0.0)
    {
        // Vuelve a calcular la posición para la extracción
        int palo_aleatorio = rand() % 4;
        int posicion_aleatoria = rand() % 10;
    }

    carta = mazo[palo_aleatorio][posicion_aleatoria]; // Se almacena el valor de la posición en el array "carta"
    mazo[palo_aleatorio][posicion_aleatoria] = 0.0;   // Se asigna el valor 0.0 a la posición donde se quitó la carta

    palo_aleatorio++;     // Se actualiza el valor de palo_aleatorio para hacer coincidir con el switch
    posicion_aleatoria++; // Se actualiza el valor de posicion_aleatoria para hacer concidir con el número correcto de carta en un mazo real

    // Setea el valor del palo de la carta según el seleccionado
    switch (palo_aleatorio)
    {
    case 1: // ORO
        strcpy(palo, "ORO");
        break;
    case 2: // COPA
        strcpy(palo, "COPA");
        break;
    case 3: // BASTO
        strcpy(palo, "BASTO");
        break;
    case 4: // ESPADA
        strcpy(palo, "ESPADA");
        break;
    }

    printf("\nCarta Extraída:\nPalo: %s - Carta: %d - Valor: %.1f\n\n", palo, posicion_aleatoria, carta); // Muestra la carta elegida
    // printf("Estado del mazo: \n");
    // imprimir_mazo(mazo); // Imprime el mazo
    // printf("\n\n");
    return carta; // Devuelve el valor de la carta
}