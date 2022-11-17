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

/**
 * Imprime el menú del jugador
 * @return devuelve como entero la opción elegida por el jugador para poder continuar
 */
int imprimir_menu_jugador(int cartas_disponibles);

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
    opcion_menu_p = imprimir_menu_principal();
    system("clear");

    // Define el accionar dependiendo de la decisión obtenida desde el menú
    if (opcion_menu_p == 1)
    {
        // Preparación para el inicio del juego
        float mazo[4][10];     // Definición del mazo
        incializar_mazo(mazo); // Inicialización del mazo
        while ((numero_jugadores < 2) || (numero_jugadores > 8))
        {
            printf("\nIngrese numero de jugadores (2 a 8): "); // Solicita ingreso de número de jugadores
            scanf("%d", &numero_jugadores);                    // Almacena el número de jugadores
            if (numero_jugadores < 2 || numero_jugadores > 8)  // Controla que el número de jugadores sea de 2 a 8
            {
                printf("\nEl numero de jugadores no es correcto.\n");
            }
        }
        iniciar_juego(numero_jugadores, mazo); // Inicia el juego
        // while (wait(NULL) != -1 || errno != ECHILD)
        // {
        // }

        // Fin del juego
        printf("\n¡ FIN DEL JUEGO !\nEsperamos que se hayan divertido.\n\n");
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
    printf("Estado del mazo: \n");
    imprimir_mazo(mazo); // Imprime el mazo
    printf("\n\n");
    return carta; // Devuelve el valor de la carta
}

/**
 * Imprime el menu principal
 * Permite al usuario seleccionar una opción
 * @return devuelve como entero (int) la opción seleccionada
 */
int imprimir_menu_principal()
{
    int eleccion;
    bool loop = true;

    while (loop)
    {
        printf("Menú principal\n1 - Iniciar Juego\n2 - Salir\n\nElegir Opción: "); // Mensaje para el operador (menu)
        scanf("%d", &eleccion);
        if (eleccion == 1 || eleccion == 2)
            loop = false;
    }

    return eleccion; // Devuelve la variable
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

    // Primer pipe - enviará la carta al jugador y aguardará una nueva decisión
    if (pipe(repartidor_a_jugador) == -1)
    {
        printf("\nError en la creación del pipe(repartidor_a_jugador).\n");
        return;
    };

    // Segundo pipe - enviará la decisión al repartidor dependiendo de su decisión
    if (pipe(jugador_a_repartidor) == -1)
    {
        printf("\nError en la creación del pipe(repartidor_a_jugador).\n");
        return;
    };

    // int CONTROL_PADRE = (2 - N); // Constante para el control

    int tamanio_mazo_actual = tamanio_mazo(mazo); // Cartas que quedan en el mazo

    pid_t shut_down[7]; // Definición del arreglo para controlar la finalización de los procesos hijos

    int pid; // Seeguimiento de procesos PADRE / HIJOS

    for (int i = 0; i < N; i++)
    {
        pid = fork(); // Llamado al sistema fork()

        // Manejo error fork
        if (pid < 0)
        {
            printf("¡ERROR! El proceso hijo %d ha fallado", i);
            return;
        }

        // Si el PID es 0, es el proceso hijo, equivalente a un jugador
        if (pid == 0)
        {
            close(repartidor_a_jugador[1]);
            // Salida que muestra cuando un jugador comienza a jugar
            printf("El Jugador %d (PID: %d) ha entrado al juego!\n", i, getpid());
            // Se inicia la función de juego
            // control_jugador(repartidor_a_jugador, jugador_a_repartidor, i);

            // Programa que controla al jugador
            int opcion_elegida;    // Almacena opción del menú de jugador (1 = Nueva Carta, 2 = Se Planta)
            float puntaje_jugador; // Inicializa el puntaje del jugador
            float carta_dada;      // Inicializa el valor de la carta otorgada al jugador

            // El jugador lee su puntaje
            if (read(repartidor_a_jugador[0], &puntaje_jugador, SIZE_OF_FLOAT) < 0)
            {
                printf("ERROR. No se pudo leer del Pipe en proceso ID %d.\n", getpid());
                return;
            }

            printf("\nSoy el jugador %d (ID: %d), mi puntaje actual es: %.2f", N, getpid(), puntaje_jugador); // Muestra al jugador participando en el juego

            // El jugador lee la carta otorgada
            if (read(repartidor_a_jugador[0], &carta_dada, SIZE_OF_FLOAT) < 0)
            {
                printf("ERROR. No se pudo leer del Pipe en proceso ID %d.\n", getpid());
                return;
            }

            printf("\nSoy el jugador %d (ID: %d), me dieron una carta con puntaje %.2f.\n", N, getpid(), carta_dada); // Muestra la carta dada al jugador

            // Suma a su mano la carta
            puntaje_jugador += carta_dada;

            // El jugador muestra su puntaje por pantalla
            printf("\nSoy el jugador %d (ID: %d), mi puntaje actual es: %.2f", N, getpid(), puntaje_jugador); // Muestra al jugador participando en el juego

            // Decide, acorde a su puntaje, si intenta sacar una carta más o no
            if (puntaje_jugador >= 7.5)
            {
                printf("\nSoy el jugador %d (ID: %d), acá me planto!\n", N, getpid()); // Muestra al jugador plantandose
                opcion_elegida = 2;                                                    // Decide no sacar más cartas = SE PLANTA
                write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);          // Escribe al repartidor con la opción elegida
                write(jugador_a_repartidor[1], &puntaje_jugador, SIZE_OF_FLOAT);       // Escribe el puntaje del jugador al repartidor
                close(jugador_a_repartidor[0]);                                        // Cierra la lectura
                close(jugador_a_repartidor[1]);                                        // Cierra la escritura
            }
            else if (puntaje_jugador < 7.5)
            {
                opcion_elegida = rand() % 2 + 1; // Decide si saca o no, aleatoriamente
                if (opcion_elegida == 2)
                {
                    printf("\nSoy el jugador %d (ID: %d), acá me planto!\n", N, getpid()); // Muestra al jugador plantandose
                    write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);          // Escribe al repartidor con la opción elegida
                    write(jugador_a_repartidor[1], &puntaje_jugador, SIZE_OF_FLOAT);       // Escribe el puntaje del jugador al repartidor
                    close(jugador_a_repartidor[0]);                                        // Cierra la lectura
                    close(jugador_a_repartidor[1]);                                        // Cierra la escritura
                }
                else
                {
                    printf("\nSoy el jugador %d (ID: %d), pido una nueva carta, por favor. \n", N, getpid()); // Muestra al jugador pidiendo carta
                    write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);                             // Escribe al repartidor con la opcion elegida
                    write(jugador_a_repartidor[1], &puntaje_jugador, SIZE_OF_FLOAT);                          // Escribe el puntaje del jugador al repartidor
                    close(jugador_a_repartidor[0]);                                                           // Cierra la lectura
                }
            }
            close(jugador_a_repartidor[1]); // Cierra la escritura

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
        close(jugador_a_repartidor[1]);
        bool loop = true;
        float carta_a_jugador = 0.0; // Almacena la carta que se da al jugador
        float puntaje_jugador = 0.0; // Almacena el puntaje del jugador
        int decision_jugador = 1;    // Decisión del jugador (1 = Nueva carta; 2 = Se planta)

        while (loop)
        {
            // Si quedan cartas disponibles
            if (tamanio_mazo_actual > 0 && decision_jugador == 1)
            {
                printf("Soy el repartidor (ID: %d).\n", getpid());
                printf("Cartas disponibles en la baraja: %d\n", tamanio_mazo_actual); // Muestra el tamaño actual de la baraja
                carta_a_jugador = saca_carta(mazo);                                   // Saca una carta del mazo de forma aleatoria
                tamanio_mazo_actual--;                                                // Disminuye en 1 el tamaño del mazo
                puntaje_jugador += carta_a_jugador;                                   // Suma el puntaje de la carta obtenida al puntaje del jugador
                write(repartidor_a_jugador[1], &carta_a_jugador, SIZE_OF_FLOAT);      // Envía la carta al jugador
                write(repartidor_a_jugador[1], &puntaje_jugador, SIZE_OF_FLOAT);      // Envía el puntaje al jugador

                wait(NULL);

                read(jugador_a_repartidor[0], &decision_jugador, SIZE_OF_INT);  // Lee la decisión del jugador
                read(jugador_a_repartidor[0], &puntaje_jugador, SIZE_OF_FLOAT); // Lee el puntaje al jugador

                if (decision_jugador == 2)
                {
                    // El jugador se planta
                    printf("Soy el repartidor (ID: %d). El jugador se planto.\n", getpid());
                    close(repartidor_a_jugador[0]);
                    close(repartidor_a_jugador[1]);
                    loop = false;
                }
            }
            else if (tamanio_mazo(mazo) <= 0)
            {
                printf("No hay cartas disponibles.\n");
                close(repartidor_a_jugador[0]);
                close(repartidor_a_jugador[1]);
                loop = false;
            }
            // VER SI LO DE ABAJO FUNCIONA
            else if (wait(NULL) != -1 || errno != ECHILD)
            {
                // No hay procesos hijos en ejecución
                loop = false;
            }
            printf("El puntaje del jugador es: %.2f\n", puntaje_jugador);
        }

        // Manejo de procesos Zombis
        for (int i = 0; i < N; i++)
        {
            printf("Esperando a que el Jugador %d (PID: %d) salga del juego.\n", i, getpid());
            waitpid(shut_down[i], NULL, 0);
            printf("El jugador %d ha salido del juego.\n", i);
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
    int opcion_elegida;    // Almacena opción del menú de jugador (1 = Nueva Carta, 2 = Se Planta)
    float puntaje_jugador; // Inicializa el puntaje del jugador
    float carta_dada;      // Inicializa el valor de la carta otorgada al jugador

    // El jugador lee su puntaje
    if (read(repartidor_a_jugador[0], &puntaje_jugador, SIZE_OF_FLOAT) < 0)
    {
        printf("ERROR. No se pudo leer del Pipe en proceso ID %d.\n", getpid());
        return;
    }

    printf("\nSoy el jugador %d (ID: %d), mi puntaje actual es: %.2f", id, getpid(), puntaje_jugador); // Muestra al jugador participando en el juego

    // El jugador lee la carta otorgada
    if (read(repartidor_a_jugador[0], &carta_dada, SIZE_OF_FLOAT) < 0)
    {
        printf("ERROR. No se pudo leer del Pipe en proceso ID %d.\n", getpid());
        return;
    }

    printf("\nSoy el jugador %d (ID: %d), me dieron una carta con puntaje %.2f.\n", id, getpid(), carta_dada); // Muestra la carta dada al jugador

    // Suma a su mano la carta
    puntaje_jugador += carta_dada;

    // El jugador muestra su puntaje por pantalla
    printf("\nSoy el jugador %d (ID: %d), mi puntaje actual es: %.2f", id, getpid(), puntaje_jugador); // Muestra al jugador participando en el juego

    // Decide, acorde a su puntaje, si intenta sacar una carta más o no
    if (puntaje_jugador >= 7.5)
    {
        printf("\nSoy el jugador %d (ID: %d), acá me planto!\n", id, getpid()); // Muestra al jugador plantandose
        opcion_elegida = 2;                                                     // Decide no sacar más cartas = SE PLANTA
        write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);           // Escribe al repartidor con la opción elegida
        write(jugador_a_repartidor[1], &puntaje_jugador, SIZE_OF_FLOAT);        // Escribe el puntaje del jugador al repartidor
        close(jugador_a_repartidor[0]);                                         // Cierra la lectura
        close(jugador_a_repartidor[1]);                                         // Cierra la escritura
    }
    else if (puntaje_jugador < 7.5)
    {
        opcion_elegida = rand() % 2 + 1; // Decide si saca o no, aleatoriamente
        if (opcion_elegida == 2)
        {
            printf("\nSoy el jugador %d (ID: %d), acá me planto!\n", id, getpid()); // Muestra al jugador plantandose
            write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);           // Escribe al repartidor con la opción elegida
            write(jugador_a_repartidor[1], &puntaje_jugador, SIZE_OF_FLOAT);        // Escribe el puntaje del jugador al repartidor
            close(jugador_a_repartidor[0]);                                         // Cierra la lectura
            close(jugador_a_repartidor[1]);                                         // Cierra la escritura
        }
        else
        {
            printf("\nSoy el jugador %d (ID: %d), pido una nueva carta, por favor. \n", id, getpid()); // Muestra al jugador pidiendo carta
            write(jugador_a_repartidor[1], &opcion_elegida, SIZE_OF_INT);                              // Escribe al repartidor con la opcion elegida
            write(jugador_a_repartidor[1], &puntaje_jugador, SIZE_OF_FLOAT);                           // Escribe el puntaje del jugador al repartidor
            close(jugador_a_repartidor[0]);                                                            // Cierra la lectura
        }
    }
    close(jugador_a_repartidor[1]); // Cierra la escritura
}
