// Se modifica la codificación del Algoritmo 3 para darle una orientación más aproximada a objetos

#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

// Se define la clase ClaseSaludoHilo
class ClaseVectorHilo
{
public:
    int dHilo, rHilo;  // Variables que definen la demora y repeticion de un hilo
    string sHilo; // Variable que almacena el saludo del hilo
    thread P[100];

    ClaseVectorHilo(int pos)
    {
        // Este constructor será utilizado para crear el vector de threads (se deberá informar la cantidad de posiciones que el vector tendrá)
        thread P[pos];
    };

    int demoraHilo(int minimo, int maximo)
    {
        // Se deberá definir el mínimo y máximo de los número aleatorios dentro de la llamada al método
        // Previamente al uso de este método se deberá generar una semilla utilizando srand()
        // Retorna el número de demora aleatoria
        return dHilo = (minimo + rand() % (maximo - minimo + 1));
    };

    int repeticionHilo(int minimo, int maximo)
    {
        // Se deberá definir el mínimo y máximo de los número aleatorios dentro de la llamada al método
        // Previamente al uso de este método se deberá generar una semilla utilizando srand()
        // Retorna el número de demora aleatoria
        return rHilo = (minimo + rand() % (maximo - minimo + 1));
    };

    int demoraEstandar(int hilo)
    {
        // Utiliza el valor del hilo actual para la generación de una demora estándar de acuerdo al número de hilo
        return (hilo + 1) * 5;
    };

    string saludoHilo(int hilo)
    {
        // Utiliza el valor del hilo actual para la generación del saludo del hilo
        string texto = "Soy el hilo ";
        texto += to_string(hilo);
        return sHilo = texto;
    };
};

void misaludo(string nombreHilo, int demoraHilo, int repeticionHilo)
{
    for (int i = 1; i <= repeticionHilo; i++)
    {
        cout << nombreHilo + "\n";
        this_thread::sleep_for(chrono::milliseconds(demoraHilo));
    }
}

int main()
{
    srand(time(0)); // Semilla generada utilizando el reloj del sistema

    int hiloAleatorio; // Se crea la variable para seleccionar el hilo que se pausará de forma aleatoria
    int cantHilos;     // Se crea una variable para seleccionar la cantidad de hilos a crear

    cout << "Elija la cantidad de hilos a crear (máximo 100): "; // Se solicita el ingreso de la cantidad de hilos a crear
    cin >> cantHilos;                               // Se asigna el numero de cantidad de hilos a la variable "cantHilos"
    cout << "\n\n";

    while (cantHilos < 1 || cantHilos > 100) // Se comprueba que la cantidad de hilos se puedan almacenar en el vector creado
    {
        cout << "La cantidad debe ser mayor a 0 y menor a 100. Ingrese nueva cantidad: "; // En caso de no poder ser almacenados, se vuelve a solicitar el dato
        cin >> cantHilos;
        cout << "\n\n";  // Se almacena nuevamente el número de hilos
    }

    ClaseVectorHilo VH(cantHilos); // Se crea el vector de threads (VH) con la cantidad de hilos definida

    cout << "Elija el hilo que se pausara de forma aleatoria: "; // Se solicita el ingreso del número de hilo
    cin >> hiloAleatorio;                                        // Se almacena el número de hilo en la variable "hiloAleatorio"
    cout << "\n\n";
    hiloAleatorio--; // Se disminuye en un valor el contenido de la variable "hiloAleatorio" para que esta pueda ser utilizada dentro del vector de threads

    while (hiloAleatorio < 0 || hiloAleatorio > cantHilos) // Se comprueba que el hilo seleccionado se encuentre dentro de los existentes
    {
        cout << "El hilo no existe, el numero de hilo deberá ser entre el 0 y el 15. Ingrese el nuevo hilo: "; // En caso de que no exista se solicita un nuevo ingreso.
        cin >> hiloAleatorio;
        cout << "\n\n";  // Se almacena nuevamente el número de hilo
        hiloAleatorio--; // Se vuelve a ajustar el valor del parámetro para poder ser utilizado en el vector de threads
    }

    // Comienzo de la creación de cada uno de los threads en el vector de threads P
    for (int i = 0; i < cantHilos; i++)
    {
        // Se identifica si el hilo a crear es el seleccionado para pausarse de forma aleatoria
        if (i != hiloAleatorio)
        {
            VH.P[i] = thread(&misaludo, VH.saludoHilo(i+1), VH.demoraHilo(100,1000), VH.repeticionHilo(5,20)); // Se crea el hilo con cadena especifica, demora y numero de ejecuciones aleatorias
        }
        else
        {
            VH.P[i] = thread(&misaludo, VH.saludoHilo(i+1), VH.demoraEstandar(i), VH.repeticionHilo(5,20)); // Se crea el hilo con los valores específicos de cadena, demora estándar y numero de ejecuciones aleatoria
        }
    }
    // Fin de la creación de los threads en el vector de treads P

    // Inicio de los threads
    for (int i = 0; i < cantHilos; i++)
    {
        VH.P[i].join();
    }
    // Fin de los threads

    cout << "\nFin \n"; // Finaliza la ejecución mostrando la cadena "Fin" por la terminal
}