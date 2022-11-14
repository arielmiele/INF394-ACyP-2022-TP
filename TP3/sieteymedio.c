/**
 * Trabajo Práctico N° 3
 * Materia: INF289 - Algoritmos Concurrentes y Paralelos
 * Alumo: MIELE, Ariel Gerardo
 * DNI: 34.434.704
 * Legajo: VINF011482
 * Año: 2022
 */

/**
 * Consigna
 *
 * Diseñar y desarrollar una aplicación que permita simular el juego de cartas del siete y medio.
 * La aplicación deberá operar con varios procesos mediante un modelo cliente/servidor, clientes = jugadores,
 * y servidor = {distribuir las cartas, recoger las decisiones de los jugadores, hasta que se declare un ganador}.
 *
 * La ejecución de la partida será mediante el comando: iniciar_juego N.
 * N será el número de jugadores que participan en la primera partida.
 * Como resultado de la ejecución, se publica el ganador de la partida,
 * así como una tabla describiendo la situación de cada jugador (cuántos puntos posee, se plantó o abandonó).
 *
 * El proceso iniciar_juego será el encargado de gestionar la partida, será el que repartirá las cartas por turnos,
 * mantendrá el estado de la partida a partir de las decisiones de los jugadores, y decidirá finalmente el ganador de la partida.
 *
 * El proceso jugador será el que implementará la lógica del jugador en la partida,
 * de acuerdo a las cartas que reciba del repartidor (iniciar_juego), y de su propio estado (puntos),
 * decidirá si continúa en el juego pidiendo más cartas, se queda ahí, o abandona (porque se pasó).
 * Esta lógica de juego se deja abierta, de forma que se implemente como cada grupo lo decida,
 * pudiendo llegar a ser simplemente aleatoria: según un número aleatorio, el jugador decide plantarse, pedir carta, o abandonar.
 *
 * Recuerda que la generación de los N procesos jugadores deberá realizarse a través del uso de forks,
 * y la comunicación repartidor/jugador se efectuará mediante pipes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

// Inicializa el mazo
void incializar_mazo(float mazo[4][10]);

/**
 * Imprime el menú principal
 * Devuelve como enterio la opción elegida para continuar
 */
int imprimir_menu_principal();

// Inicia el juego (N = cantidad de jugadores)
void iniciar_juego(int N);

// Controla el accionar del jugador
void control_jugador(int *repartidor_a_jugador, int *jugador_a_repartidor, int id);

// Devuelve un valor al azar de entre las cartas que no se han sacado
// A partir del arreglo de la baraja
float saca_carta(float mazo[4][10]);

// Muestra el tamaño del mazo
int tamanio_mazo(float mazo[4][10]);

const int NUMBER_MIN_PLAYERS = 2;    // Cantidad jugadores mínimos
const int NUMBER_MAX_PLAYER = 8;     // Cantidad jugares máximos
const int SIZE_OF_INT = sizeof(int); // Parámetro requerido para enviar enteros por el pipe
const int TAMANIO_MAZO_INICIAL = 40; // Baraja/Mazo = 40 cartas

int main(int argc, char const *argv[])
{
    // Se inicia la semilla para la generación de números aleatorios
    srand(time(0));

    int baraja[39];

    int numero_jugadores = 0;

    int opcion_menu_p;

    // Imprime saludo inicial
    printf("¡ Bienvenido a 7 y medio !\nEsperamos que se divierta.\n");

    // Imprimir menú principal
    opcion_menu_p = imprimir_menu_principal;

    if (opcion_menu_p == 1)
    {
        float mazo[4][10];
        incializar_mazo(mazo);
        while (numero_jugadores < 2 && numero_jugadores > 8)
        {
            printf("Ingrese numero de jugadores (2 a 8): ");
            scanf("%d", numero_jugadores);
            if (numero_jugadores < 2 || numero_jugadores > 8)
            {
                printf("El numero de jugadores no es correcto.\n");
            }
        }
        iniciar_juego(numero_jugadores); // Inicia el juego
        printf("\n¡ FIN DEL JUEGO !\nEsperamos que se hayan divertido.\n");
        return 0;
    }
    else if (opcion_menu_p == 2)
    {
        printf("\n¡ FIN DEL JUEGO !\nEsperamos que se hayan divertido.\n");
        return 0;
    }

    system("pause");
    return 0;
}

// Inicializa el mazo
void incializar_mazo(float mazo[4][10])
{
    /**
     * Estructura del Mazo (filas):
     * 1 = ORO
     * 2 = COPA
     * 3 = BASTO
     * 4 = ESPADA
     *
     * El mazo no tiene 8 y 9
     * Si la posición contiene un 0, esa carta no está en el mazo
     */

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (j <= 6)
            {
                mazo[i][j] = 1.0; // Asigna valor 1 a las cartas del 1 al 7
            }
            else
            {
                mazo[i][j] = 0.5; // Asigna valor 0.5 a las cartas con figuras
            }
        }
    }
    printf("¡MAZO LISTO! Comienza el juego...\n");
    return;
}

int tamanio_mazo(float mazo[4][10])
{
    int tamanio = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (mazo[i][j] != 0)
            {
                tamanio++;
            }
        }
    }

    return tamanio;
}

float saca_carta(float mazo[4][10])
{
    float carta = 0.0;
    int palo_aleatorio = 0 + rand() % (4 - 0 + 1);
    return carta;
}

// Impresión del menú principal
int imprimir_menu_principal()
{
    int eleccion = 0;

    while (eleccion != 1 || eleccion != 2)
    {
        printf("Menú principal\n1 - Iniciar Juego\n2 - Salir\n\nElegir Opción: ");
        scanf("%d", eleccion);
    }
    return eleccion;
}

// Impresión del menú de jugador
int imprimir_menu_jugador(int cartas_disponibles)
{
    int opcion;
    printf("Quedan %d cartas disponibles.\n", cartas_disponibles);
    printf("Menu del Jugador:\n1 - Tomar carta\n2 - Plantarse\nElegir Opción: ");
    scanf("%d", opcion);
    return opcion;
}

// Inicia la partida con comando "iniciar_juego N"
void iniciar_juego(int N)
{
    // Programa que controla el juego - desde el lado del Servidor
    // Servidor = Reparte cartas, recoge decisiones, define ganador
    // Ciclo para la creación de hijos

    int repartidor_a_jugador[2]; // FD del primer pipe
    int jugador_a_repartidor[2]; // FD del segundo pipe
    pipe(repartidor_a_jugador);  // Primer pipe
    pipe(jugador_a_repartidor);  // Segundo pipe

    int CONTROL_PADRE = (2 - N); // Constante para el control

    int tamanio_mazo_actual = TAMANIO_MAZO_INICIAL; // Cartas que quedan en el mazo

    pid_t shut_down[40];

    int pid; // Seeguimiento de procesos PADRE / HIJOS

    for (int i = 0; i < N; i++)
    {
        pid = fork(); // Llamado al sistema fork()

        // Manejo error fork
        if (pid < 0)
        {
            printf("¡ERROR! El hijo %d ha fallado", i);
            return -1;
        }

        // Si el PID es 0, es el proceso hijo, equivalente a un jugador
        if (pid == 0)
        {
            // Salida que muestra cuando un jugador comienza a jugar
            printf("El Jugador %d (PID: %d) ha entrado al juego!\n", i, getpid());
            // Se inicia la función de juego
            control_jugador(repartidor_a_jugador, jugador_a_repartidor, i);
            break; // Se frena la creación de hijos
        }
        else
        {
            shut_down[i] = pid; // Es el arreglo de PIDs para esperar a que finalicen los hijos
        }
    } // Fin del ciclo de creación de hijos

    // Si el pid es mayor a 0, se ejecuta el proceso padre
    if (pid > 0)
    {
        bool loop = true;
        while (loop)
        {
            write(repartidor_a_jugador[1], &tamanio_mazo_actual, SIZE_OF_INT);
            read(jugador_a_repartidor[0], &tamanio_mazo_actual, SIZE_OF_INT);

            // Si quedan cartas disponibles
            if (tamanio_mazo_actual > 0)
                printf("Cartas disponibles en la baraja: %d\n", tamanio_mazo_actual);

            // Cuando tamanio_mazo_actual < CONTROL_PADRE, todos los hijos terminaron
            if (tamanio_mazo_actual < CONTROL_PADRE)
            {
                loop = false;
            }
        }

        // Manejo de procesos Zombis
        for (int i = 0; i < N; i++)
        {
            printf("Esperando a que el Jugador %d (PID: %d) salga del juego.\n", i, getpid());
            waitpid(shut_down[i], NULL, 0);
            printf("El jugador %d (PID: %d) ha salido del juego.\n", i, getpid());
        }

        // Comprobación de que todos los jugadores han terminado de jugar
        if (pid > 0)
            printf("Todos los jugadores han finalizado de jugar.\n");
        return;
    }
}

void control_jugador(int *repartidor_a_jugador, int *jugador_a_repartidor, int id)
{
    // Programa que controla al jugador
    bool loop = true;
    bool plantado = false;
    int opcion_elegida; // Almacena opción del menú de jugador

    while (loop)
    {
        int cartas_disponibles = 0; // Variable local de cartas disponibles

        // Espera a que el padre escriba, luego procesa las cartas disponibles
        read(repartidor_a_jugador[0], &cartas_disponibles, SIZE_OF_INT);

        // Si quedan cartas disponibles, escribir la disponibilidad y luego tomar una
        if (cartas_disponibles > 0)
        {
            opcion_elegida = imprimir_menu_jugador;
            if (opcion_elegida == 1)
            {
                // Retirar una carta
                cartas_disponibles--;
            }
            else if (opcion_elegida == 2)
            {
                // El jugador se planta
                plantado = true;
                loop = false;
            }

            if (cartas_disponibles == 0)
            {
                printf("¡No hay cartas disponibles!\n");
                loop = false;
            }

            // El jugador escribe al repartidor cuantas cartas quedan y detiene la ejecución
            write(jugador_a_repartidor[1], &cartas_disponibles, SIZE_OF_INT);
            usleep(100); // Detener la ejecución por un intervalo de tiempo
        }

        // Si no quedan cartas, se detiene el bucle, y escribe nuevamente al repartidor
        else
        {
            loop = false;
            cartas_disponibles--;
            write(jugador_a_repartidor[1], &cartas_disponibles, SIZE_OF_INT);
        }
    }
}
