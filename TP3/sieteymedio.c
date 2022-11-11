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
 * Diseña y desarrolla una aplicación que permita simular el juego de cartas del siete y medio. La aplicación deberá operar con varios procesos mediante un modelo cliente/servidor, clientes = jugadores, y servidor = {distribuir las cartas, recoger las decisiones de los jugadores, hasta que se declare un ganador}.
 *
 * La ejecución de la partida será mediante el comando: iniciar_juego N. N será el número de jugadores que participan en la primera partida. Como resultado de la ejecución, se publica el ganador de la partida, así como una tabla describiendo la situación de cada jugador (cuántos puntos posee, se plantó o abandonó).
 *
 * El proceso iniciar_juego será el encargado de gestionar la partida, será el que repartirá las cartas por turnos, mantendrá el estado de la partida a partir de las decisiones de los jugadores, y decidirá finalmente el ganador de la partida.
 *
 * El proceso jugador será el que implementará la lógica del jugador en la partida, de acuerdo a las cartas que reciba del repartidor (iniciar_juego), y de su propio estado (puntos), decidirá si continúa en el juego pidiendo más cartas, se queda ahí, o abandona (porque se pasó). Esta lógica de juego se deja abierta, de forma que se implemente como cada grupo lo decida, pudiendo llegar a ser simplemente aleatoria: según un número aleatorio, el jugador decide plantarse, pedir carta, o abandonar.
 *
 * Recuerda que la generación de los N procesos jugadores deberá realizarse a través del uso de forks, y la comunicación repartidor/jugador se efectuará mediante pipes.
 *
 */

int main(int argc, char const *argv[])
{
    // Clientes = Jugadores
    // Servidor = Reparte cartas, recoge decisiones, define ganador

    // Inicia la partida con comando "iniciar_juego N"
    // "N" es el número de jugadores que participan en la partida

    // Se publica el ganador de la partida y una tabla de la situación de cada jugador (puntos, plantó, abandonó)

    // Los N procesos jugadores se tienen que crear usando forks

    // La comunicación entre repartidor y jugador se hará mediante pipes
    return 0;
}
