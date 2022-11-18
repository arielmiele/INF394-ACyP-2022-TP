#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

// Inicializa el mazo
void incializar_mazo(float mazo[4][10]);

// Devuelve un valor al azar de entre las cartas que no se han sacado a partir del arreglo de la baraja
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

    printf("\nEl numero de jugadores es: %d.\n\n", numJugadores);

    float mazo[4][10];     // Definición del mazo
    incializar_mazo(mazo); // Inicialización del mazo
    imprimir_mazo(mazo);

    int numPipes = numJugadores * 2; // Calcula el número de pipes que deberán usarse
    int pids[numJugadores];          // Almacena los pid de los jugadores

    // Creación de los file descriptors para cada uno de los pipes
    // Los file descriptors de los pipes se asignarán de a pares para las conexiones con cada uno de los jugadores (ida y vuelta)
    int rep_a_jug[numJugadores][2]; // Pipe para comunicación de repartidor a jugador
    int i;
    for (i = 0; i < numJugadores; i++)
    {
        if (pipe(rep_a_jug[i]) < 0)
        {
            printf("\nProblema abriendos los pipes de Repartidor a Jugador.\n");
            return 1;
        }
    }

    int quiero_cartas[numJugadores][2]; // Pipe para comunicación de repartidor a jugador
    for (i = 0; i < numJugadores; i++)
    {
        if (pipe(quiero_cartas[i]) < 0)
        {
            printf("\nProblema abriendos los pipes de Repartidor a Jugador.\n");
            return 1;
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

            // Proceso perteneciente al hijo
            float puntajeJ = 0.0;  // Inicializa el puntaje del jugador en 0.0
            float puntajeC;        // Inicializa el puntaje de la carta entregada por el repartidor
            int decisionCarta = 0; // Decisión por una nueva carta
            bool loop = true;      // Controla el loop de pedido de cartas

            while (loop)
            {
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

                if (puntajeJ > 7.5)
                {
                    printf("\nSoy el Jugador %d (ID: %d), acabo de perder =(.\n", i + 1, getpid());
                    decisionCarta = 0;
                    if (write(quiero_cartas[i][1], &decisionCarta, SIZE_OF_INT) == -1)
                    {
                        printf("\nError en la escritura del pipe.\n");
                        return 5;
                    }
                    loop = false;
                }
                else if (puntajeJ < 7.5)
                {
                    // Decide si quiere una nueva carta o no
                    decisionCarta = rand() % 2; // 0 = sin nueva carta ; 1 = pide nueva carta
                    if (decisionCarta == 1)
                    {
                        printf("\nSoy el Jugador %d (ID: %d), pido una nueva carta.\n", i + 1, getpid());
                        if (write(quiero_cartas[i][1], &decisionCarta, SIZE_OF_INT) == -1)
                        {
                            printf("\nError en la escritura del pipe.\n");
                            return 5;
                        }
                        sleep(1);
                    }
                    else if (decisionCarta == 0)
                    {
                        printf("\nSoy el Jugador %d (ID: %d), me planto.\n", i + 1, getpid());
                        if (write(quiero_cartas[i][1], &decisionCarta, SIZE_OF_INT) == -1)
                        {
                            printf("\nError en la escritura del pipe.\n");
                            return 5;
                        }
                        loop = false;
                    }
                }
                else if (puntajeJ == 7.5)
                {
                    printf("\nSoy el Jugador %d (ID: %d), me planto.\n", i + 1, getpid());
                    decisionCarta = 0;
                    if (write(quiero_cartas[i][1], &decisionCarta, SIZE_OF_INT) == -1)
                    {
                        printf("\nError en la escritura del pipe.\n");
                        return 5;
                    }
                    loop = false;
                }
            }

            // Mensaje de salida del jugador
            printf("\nEl Jugador %d (ID: %d) ha finalizado su juego.\n", i + 1, getpid());

            return 0; // Finaliza la creación de procesos hijos (jugadores)
        }
    }

    // Proceso padre

    sleep(1);

    printf("\nEl Repartidor (ID: %d) ha entrado al juego.\n", getpid());

    float puntajes[numJugadores]; // Arreglo para almacenar los puntajes de los jugadores

    for (int j = 0; j < numJugadores; j++)
    {
        puntajes[j] = 0.0;
    }

    float puntosCarta = 0.0;    // Inicializa los puntos de la carta a enviar
    float puntajeJugador = 0.0; // Inicializa el puntaje del jugador
    int quiereCarta;            // Registra la info del jugador pidiendo cartas
    bool loopR;
    // Envía la carta al proceso
    for (int i = 0; i < numJugadores; i++)
    {
        loopR = true;
        while (loopR)
        {
            // Saca una carta del mazo
            puntosCarta = saca_carta(mazo);
            puntajes[i] += puntosCarta;

            // Envía la carta por el pipe
            if (write(rep_a_jug[i][1], &puntosCarta, SIZE_OF_FLOAT) == -1)
            {
                printf("\nError en la escritura del pipe.\n");
                return 6;
            }

            if (read(quiero_cartas[i][0], &quiereCarta, SIZE_OF_INT) == -1)
            {
                printf("\nError en la lectura del pipe.\n");
                return 7;
            }

            if (quiereCarta == 1)
            {
                printf("\nJugador %d pide una nueva carta.\n", i + 1);
            }
            else if (quiereCarta == 0)
            {
                printf("\nJugador %d plantado, no se envian mas cartas.\n", i + 1);
                loopR = false;
                waitpid(pids[i], NULL, 0);
            }

            // Agrega los puntajes al array de puntajes
            // puntajes[i] += puntajeJugador;
        }
    }

    // Espera que todos los jugadores finalicen sus turnos
    for (i = 0; i < numJugadores; i++)
    {
        wait(NULL);
    }

    int mayorPuntaje = 0;        // Controla el valor de puntaje mayor
    int ganadores[numJugadores]; // Arreglo para almacenar los ganadores (1 = ganador, 0 = perdedor)
    int cantGanadores = 0;       // Controla la cantidad de ganadores
    int posGanador = 0;          // Define la posición del ganador

    for (int j = 0; j < numJugadores; j++)
    {
        ganadores[j] = 0;
    }

    // Se muestran los puntos de los jugadores
    printf("\nJUEGO TERMINADO - Tabla de Resultados:\n");

    for (i = 0; i < numJugadores; i++)
    {
        printf("Proceso [%d] - Jugador [%d]: %d - Puntaje Final: %.2f\n", i, i + 1, pids[i], puntajes[i]);
        if (mayorPuntaje < puntajes[i] && puntajes[i] <= 7.5)
        {
            mayorPuntaje = puntajes[i];
            cantGanadores = 1;
            posGanador = i;
        }
        else if (mayorPuntaje == puntajes[i])
        {
            ganadores[i] = 1;
            cantGanadores++;
        }
    }

    if (cantGanadores == 1)
    {
        printf("\n** El ganador es el Jugador %d, con un puntaje de %.2f. ¡ FELICITACIONES ! **\n", posGanador + 1, puntajes[posGanador]);
    }
    else
    {
        printf("\n¡ HAY UN EMPATE !, los ganadores son: \n");
        for (i = 0; i < numJugadores; i++)
        {
            if (ganadores[i] == 1)
            {
                printf("Jugador %d, con un puntaje de %.2f.\n", i + 1, puntajes[i]);
            }
        }
        printf("\n¡ FELICITACIONES !\n");
    }

    printf("\nMuchas gracias por jugar, esperamos que se hayan divertido!\n");
    printf("\n");

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
        palo_aleatorio = rand() % 4;
        posicion_aleatoria = rand() % 10;
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

    printf("\nCarta Extraída:\nPalo: %s - Carta: %d - Valor: %.1f\n", palo, posicion_aleatoria, carta); // Muestra la carta elegida

    return carta; // Devuelve el valor de la carta
}