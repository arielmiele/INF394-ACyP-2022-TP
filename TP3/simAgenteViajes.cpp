/**
 * Este programa genera 6 "agentes de viajes" como procesos secundarios de la función principal (proceso padre o principal).
 * Cada proceso tiene acceso a los asientos de un vuelo y puede reservarlos.
 * El programa protegerá los asientos de otros agentes de viajes para evitar problemas de sobreventa.
 * Esto se logra utilizando comunicación entre procesos mediante pipes.
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

const int NUMBER_OF_CHILDREN = 6; // Constante con el número de agente de viajes

const int PARENT_CONTROL = (2 - NUMBER_OF_CHILDREN); // Constante para el control

const int SIZE_OF_INT = sizeof(int); // Parámetro requerido para enviar enteros por el pipe

const int INITIAL_NUMBER_OF_SEATS = 100; // Constante con el número de asientos iniciales del vuelo

// Declaración de la función travel agent

void travel_agent(int *child_to_parent, int *parent_to_child, int id);

int main()
{
    int child_to_parent[2]; // Arreglo file descriptor del primer pipe
    int parent_to_child[2]; // File descriptor del segundo pipe
    pipe(child_to_parent);  // Primer pipe
    pipe(parent_to_child);  // Segundo pipe

    pid_t shut_down[NUMBER_OF_CHILDREN]; // Arreglo de 6 pidID para unir al final del programa

    int seats_left = INITIAL_NUMBER_OF_SEATS; // Asientos restantes de reservar

    int pid; // Para realizar un seguimiento si estamos en el proceso padre del hijo por cada fork()

    // Ciclo for para crear los agentes de viajes
    for (int i = 0; i < NUMBER_OF_CHILDREN; i++)
    {
        pid = fork(); // Llamada al sistema fork

        // Manejo de error fork
        if (pid < 0)
        {
            cout << "ERROR! El hijo " << i << " ha fallado";
            return -1;
        }

        // Si el pid es 0, estamos en el proceso hijo
        if (pid == 0)
        {
            // Salida que muestra cuando un proceso hijo es creado
            cout << "HIJO: " << i << " CREADO " << getpid() << endl;

            // Iniciar la función travel_agent
            travel_agent(child_to_parent, parent_to_child, i);
            break; // Cortar generación de más hijos (hijos de hijos)
        }
        else
        {
            shut_down[i] = pid; // Arreglo de pids para esperar que finalicen los hijos
        }
    } // Fin del ciclo de creación de hijos

    // Si el pid es mayor a 0, se ejecuta el proceso padre
    if (pid > 0)
    {
        bool loop = true;

        // in a loop
        while (loop)
        {
            write(parent_to_child[1], &seats_left, SIZE_OF_INT);
            read(child_to_parent[0], &seats_left, SIZE_OF_INT);

            // Si quedan asientos muestra cuantos quedan
            if (seats_left > 0)
                cout << "Asientos disponibles: " << seats_left << endl;

            // Cuando seats_left < PARENT_CONTROL, todos los hijos terminaron
            if (seats_left < PARENT_CONTROL)
            {
                loop = false;
                cout << "Salida del lopp del Padre. PID: " << getpid() << endl;
            }
        }

        // Manejo de procesos Zombis
        for (int i = 0; i < NUMBER_OF_CHILDREN; i++)
        {
            cout << "Esperando al PID: " << shut_down[i] << " que termine" << endl;
            waitpid(shut_down[i], NULL, 0);
            cout << "PID: " << shut_down[i] << " ha terminado" << endl;
        }
    }

    // Comprobación de que todos los hijos terminaron
    cout << "Nos unimos todos? Si fue asi, deberiamos ser 7. PID: " << getpid() << endl;
    if (pid > 0)
        cout << "No hay ZOMBIS!" << endl;
    return 0;
}

// Función para cada agente de viaje
void travel_agent(int *child_to_parent, int *parent_to_child, int id)
{
    bool loop = true;

    // do a loop
    while (loop)
    {
        int seats_left = 0; // Variable local de asientos disponibles

        // Esperar a que el padre escriba, luego procesar los asientos restantes
        read(parent_to_child[0], &seats_left, SIZE_OF_INT); // LECTURA

        // Si quedan asientos, escribir cuantos y luego "reservar" un asiento
        // Declaracion del numero de thread y pid
        if (seats_left > 0)
        {
            cout << "Soy un HIJO: " << id << ". Hay: " << seats_left << " asientos, reservando uno!";
            cout << " Mi PID es: " << getpid() << endl;
            seats_left--;

            // Si no quedan asientos por reservar, detiene el ciclo
            if (seats_left == 0)
                loop = false;

            // Hijo escribe al padre cuantos asientos quedan y detiene la ejecución con un usleep
            write(child_to_parent[1], &seats_left, SIZE_OF_INT);
            usleep(100); // detener la ejecución por un intervalo de tiempo
        }

        // Si no quedan asientos detener el bucle, y escribir nuevamente al padre
        else
        {
            loop = false;
            seats_left--;
            write(child_to_parent[1], &seats_left, SIZE_OF_INT);
        }
    }
}