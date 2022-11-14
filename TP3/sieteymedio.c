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
#include <errno.h>

/**
 * Inicializa el mazo
 */
void incializar_mazo(float mazo[4][10]);

/**
 * Imprime el menú principal
 * @return devuelve como entero la opción elegida para continuar
 */
int imprimir_menu_principal();

// Inicia el juego (N = cantidad de jugadores)
void iniciar_juego(int N, float mazo[4][10]);

// Controla el accionar del jugador
void control_jugador(int *repartidor_a_jugador, int *jugador_a_repartidor, int id);

// Devuelve un valor al azar de entre las cartas que no se han sacado
// A partir del arreglo de la baraja
float saca_carta(float mazo[4][10]);

// Muestra el tamaño del mazo
int tamanio_mazo(float mazo[4][10]);

// Imprime el mazo
void imprimir_mazo(float mazo[4][10]);

/**
 * Declaración de constantes
 */
const int NUMBER_MIN_PLAYERS = 2;            // Cantidad jugadores mínimos
const int NUMBER_MAX_PLAYER = 8;             // Cantidad jugares máximos
const int SIZE_OF_FLOAT = sizeof(float);     // Parámetro requerido para enviar float por el pipe
const int SIZE_OF_INT = sizeof(int);         // Parámetro requerido para enviar int por el pipe
const int SIZE_OF_ARR = sizeof(char[4][10]); // Parámetro requerido para enviar la baraja por el pipe
// const int TAMANIO_MAZO_INICIAL = 40;         // Baraja/Mazo = 40 cartas

/**
 * Programa principal
 */
int main(int argc, char const *argv[])
{
    // Se inicia la semilla para la generación de números aleatorios
    srand(time(0));

    // Inicializa variables
    // int baraja[39]; // Baraja de 40 cartas

    int numero_jugadores = 0; // Almacenará el número de jugadores

    int opcion_menu_p; // Almacenará la opción elegida del menú principal

    // Imprime saludo inicial
    printf("¡ Bienvenido a 7 y medio !\nEsperamos que se divierta.\n");

    // Imprimir menú principal
    opcion_menu_p = imprimir_menu_principal;

    // Define el accionar dependiendo de la decisión obtenida desde el menú
    if (opcion_menu_p == 1)
    {
        // Preparación para el inicio del juego
        float mazo[4][10];     // Definición del mazo
        incializar_mazo(mazo); // Inicialización del mazo
        while (numero_jugadores < 2 && numero_jugadores > 8)
        {
            printf("Ingrese numero de jugadores (2 a 8): ");  // Solicita ingreso de número de jugadores
            scanf("%d", numero_jugadores);                    // Almacena el número de jugadores
            if (numero_jugadores < 2 || numero_jugadores > 8) // Controla que el número de jugadores sea de 2 a 8
            {
                printf("El numero de jugadores no es correcto.\n");
            }
        }
        iniciar_juego(numero_jugadores); // Inicia el juego

        // Fin del juego
        printf("\n¡ FIN DEL JUEGO !\nEsperamos que se hayan divertido.\n");
        return 0;
    }
    else if (opcion_menu_p == 2)
    {
        // Fin del juego
        printf("\n¡ FIN DEL JUEGO !\nEsperamos que se hayan divertido.\n");
        return 0;
    }

    system("pause");
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
    int palo_aleatorio = rand() % 5;      // Calcula el palo de la carta (de 1 a 4), asociado a las filas de la matriz MAZO
    int posicion_aleatoria = rand() % 11; // Calcula la carta de forma aleatoria (de 1 a 10), asociado a las columnas de la matriz MAZO
    char palo[10];                        // Almacena el dato del palo para mostrar por pantalla

    // Extrae una carta del mazo hasta encontrar una carta disponible (distinto de 0.0)
    while (mazo[palo_aleatorio][posicion_aleatoria] == 0.0)
    {
        // Vuelve a calcular la posición para la extracción
        int palo_aleatorio = rand() % 5;
        int posicion_aleatoria = rand() % 11;
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
    return carta;                                                                                         // Devuelve el valor de la carta
}

/**
 * Imprime el menu principal
 * Permite al usuario seleccionar una opción
 * @return devuelve como entero (int) la opción seleccionada
 */
int imprimir_menu_principal()
{
    int eleccion = 0; // Declaración de la variable que almacena la decisión tomada

    // Controla que la decisión pertenezca a las disponibles para elegir
    while (eleccion != 1 || eleccion != 2)
    {
        printf("Menú principal\n1 - Iniciar Juego\n2 - Salir\n\nElegir Opción: "); // Mensaje para el operador (menu)
        scanf("%d", eleccion);                                                     // Almacena la decisión en la variable "eleccion"
    }
    return eleccion; // Devuelve la variable
}

/**
 * Imprime el menu para que el jugador tome la decisión
 * Permite al usuario (jugador) seleccionar una opción
 * @return devuelve como entero (int) la opción seleccionada
 */
int imprimir_menu_jugador(int cartas_disponibles)
{
    int opcion;                                                    // Declaración de la variable que almacena la decisión tomada
    printf("Quedan %d cartas disponibles.\n", cartas_disponibles); // Muestra la cantidad de cartas disponibles
    printf("Menu del Jugador:\n1 - Tomar carta\n2 - Plantarse\nElegir Opción: ");
    scanf("%d", opcion); // Almacena la decisión del jugador
    return opcion;       // Devuelve la opción seleccionada
}

/**
 * Inicia el juego
 * Programa que controla el juego - desde el lado del Servidor
 * Servidor = Reparte cartas, recoge decisiones, define ganador
 */
void iniciar_juego(int N, float mazo[4][10])
{
    // Ciclo para la creación de hijos
    int repartidor_a_jugador[2]; // FD del primer pipe
    int jugador_a_repartidor[2]; // FD del segundo pipe
    pipe(repartidor_a_jugador);  // Primer pipe - enviará la carta al jugador y aguardará una nueva decisión
    pipe(jugador_a_repartidor);  // Segundo pipe - enviará la decisión al repartidor dependiendo de su decisión

    int CONTROL_PADRE = (2 - N); // Constante para el control

    int tamanio_mazo_actual = tamanio_mazo(mazo); // Cartas que quedan en el mazo

    int decision_jugador = 1; // Decisión del jugador (1 = Nueva carta; 2 = Se planta)

    pid_t shut_down[40]; // Definición del arreglo para controlar la finalización de los procesos hijos

    int pid; // Seeguimiento de procesos PADRE / HIJOS

    for (int i = 0; i < N; i++)
    {
        pid = fork(); // Llamado al sistema fork()

        // Manejo error fork
        if (pid < 0)
        {
            printf("¡ERROR! El proceso hijo %d ha fallado", i);
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
        float carta_a_jugador = 0.0; // Almacena la carta que se da al jugador
        while (loop)
        {

            // Si quedan cartas disponibles
            if (tamanio_mazo_actual > 0 && decision_jugador == 1)
            {
                printf("Cartas disponibles en la baraja: %d\n", tamanio_mazo_actual);
                carta_a_jugador = saca_carta(mazo);                              // Saca una carta del mazo de forma aleatoria
                write(repartidor_a_jugador[1], &carta_a_jugador, SIZE_OF_FLOAT); // Envía la carta al jugador
                read(jugador_a_repartidor[0], &decision_jugador, SIZE_OF_INT);   // Lee la decisión del jugador
                // wait();                                                          // Espera a que finalice uno de los jugadores
            }
            else if (tamanio_mazo <= 0)
            {
                printf("No hay cartas disponibles.\n");
                loop = false;
            }
            else if (decision_jugador == 2)
            {
                printf("El jugador se planta.\n");
                loop = false;
            }
            // VER SI LO DE ABAJO FUNCIONA
            else if (wait(NULL) != -1 || errno != ECHILD)
            {
                // No hay procesos hijos en ejecución
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

/**
 * Programa que controla el accionar del jugador
 * Toma de decisiones de forma automática
 */
void control_jugador(int *repartidor_a_jugador, int *jugador_a_repartidor, int id)
{
    // Programa que controla al jugador
    bool loop = true;
    bool plantado = false;
    int opcion_elegida;      // Almacena opción del menú de jugador
    int puntaje_jugador = 0; // Inicializa el puntaje del jugador
    float carta_dada = 0.0;  // Inicializa el valor de la carta otorgada al jugador

    while (loop)
    {
        int cartas_disponibles = 0; // Variable local de cartas disponibles

        // Espera a que el padre escriba, luego procesa las cartas disponibles
        read(repartidor_a_jugador[0], &carta_dada, SIZE_OF_FLOAT);

        // Si quedan cartas disponibles, escribir la disponibilidad y luego tomar una
        if (cartas_disponibles > 0)
        {
            opcion_elegida = imprimir_menu_jugador; // Muestra el menu al jugador para que tome la decisión
            if (opcion_elegida == 1)                // Retirar una carta
            {
                cartas_disponibles--; // Disminuye en 1 la cantidad de cartas disponibles
            }
            else if (opcion_elegida == 2) // El jugador se planta
            {
                plantado = true;
                loop = false;
            }

            if (cartas_disponibles == 0)
            {
                printf("¡No hay cartas disponibles!\n");
                loop = false;
            }

            // El jugador escribe al repartidor cuantas cartas quedan y detiene la ejecución
            write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);
            usleep(100); // Detener la ejecución por un intervalo de tiempo
        }

        // Si no quedan cartas, se detiene el bucle, y escribe nuevamente al repartidor
        else
        {
            loop = false;
            cartas_disponibles--;
            write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);
        }
    }
}
