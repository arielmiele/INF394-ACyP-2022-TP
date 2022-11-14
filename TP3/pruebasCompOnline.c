#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

void inicializar_mazo(float mazo[4][10]);

void imprimir_mazo(float mazo[4][10]);

int tamanio_mazo(float mazo[4][10]);

float retirar_carta(float mazo[4][10]);

int main()
{
    srand(time(0));

    float mazo[4][10];
    
    float puntajeJ1 = 0.0;

    inicializar_mazo(mazo);
    
    imprimir_mazo(mazo);
    
    printf("\nEl tamaño del mazo es: %d\n", tamanio_mazo(mazo));
    
    puntajeJ1 += retirar_carta(mazo);
    
    printf("\nEl puntaje del Jugador 1 es: %.2f\n", puntajeJ1);
    
    puntajeJ1 += retirar_carta(mazo);
    
    printf("\nEl puntaje del Jugador 1 es: %.2f\n\n", puntajeJ1);
    
    imprimir_mazo(mazo);
    
    printf("\nEl tamaño del mazo es: %d\n", tamanio_mazo(mazo));

    return 0;
}

void inicializar_mazo(float mazo[4][10]){
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (j <= 6)
            {
                mazo[i][j] = (float)j+1.0; // Asigna valor 1 a las cartas del 1 al 7
            }
            else
            {
                mazo[i][j] = 0.5; // Asigna valor 0.5 a las cartas con figuras
            }
        }
    }
    printf("¡MAZO LISTO! Comienza el juego...\n\n");
}

void imprimir_mazo(float mazo[4][10]){
    // Imprime el encabezado
    printf("\t1\t2\t3\t4\t5\t6\t7\tC\tS\tR\n");
    for (int i = 0; i < 4; i++)
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
        for (int j = 0; j < 10; j++)
        {
            printf("%.1f\t", mazo[i][j]);
        }
        printf("\n");
    }
}

int tamanio_mazo(float mazo[4][10]){
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

float retirar_carta(float mazo[4][10]){
    float carta = 0.0;
    int palo_aleatorio = rand() % 5;
    int posicion_aleatoria = rand() % 11;
    char palo[10];

    while (mazo[palo_aleatorio][posicion_aleatoria] == 0)
    {
        int palo_aleatorio = rand() % 5;
        int posicion_aleatoria = rand() % 11;
    }

    carta = mazo[palo_aleatorio][posicion_aleatoria];
    mazo[palo_aleatorio][posicion_aleatoria] = 0;

    /**
     * Estructura del Mazo (filas):
     * 1 = ORO
     * 2 = COPA
     * 3 = BASTO
     * 4 = ESPADA
     */
    palo_aleatorio++;
    posicion_aleatoria++;

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

    printf("\nCarta Extraída:\nPalo: %s - Carta: %d - Valor: %.1f\n\n", palo, posicion_aleatoria, carta);
    return carta;
}
