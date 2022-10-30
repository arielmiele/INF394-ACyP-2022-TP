#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

void misaludo(string m, int demora, int veces)
{
    for (int i = 1; i <= veces; i++)
    {
        cout << m + "\n";
        this_thread::sleep_for(chrono::milliseconds(demora));
    }
}

int demoraAleatoria()
{
    // La demora aleatoria se definirá entre 100 (mínima demora de los threads) y 3000 (equivalente a 3 segundos, muy por encima de la máxima)
    // Se busca entonces evidenciar con diferentes ejecuciones la demora elevada en el hilo seleccionado

    // Se define la variable para almacenar la demora aleatoria, una variable para el valor mínimo y una para el valor máximo (milisegundos)
    int dAleatoria, minD = 100, maxD = 3000;

    // Retorna el número de demora aleatoria
    return dAleatoria = minD + rand() % (maxD - minD + 1);
}

int vecesAleatoria()
{
    // Este parámetro definirá las veces que se ejecutará un hilo de forma aleatoria entre 5 y 20

    // Se definen las variables para almacenar las veces aleatorias, el mínimo y el máximo
    int vAleatoria, minV = 5, maxV = 20;

    // Retorna el número de veces aleatoria
    return vAleatoria = minV + rand() % (maxV - minV + 1);
}

int main()
{
    srand(time(0)); // Semilla generada utilizando el reloj del sistema

    thread P[15]; // Se crea el vector de threads con 15 posiciones

    int hiloAleatorio; // Se crea la variable para seleccionar el hilo que se pausará de forma aleatoria

    int dA, vA, dE; // Se crean variables para almacenar la demora y las repeticiones aleatorias para que puedan ser mostradas por pantalla. Se crea variable dE para el cálculo de la demora estándar.

    string texto = "Soy el proceso ", textoM = ""; // Se crea la cadena estándar para ser mostrada al ususario y la variable temporal para modificarlo "textoM"

    cout << "Elija el hilo que se pausara de forma aleatoria: "; // Se solicita el ingreso del número de hilo
    cin >> hiloAleatorio;                                        // Se almacena el número de hilo en la variable "hiloAleatorio"
    cout << "\n\n";
    hiloAleatorio--; // Se disminuye en un valor el contenido de la variable "hiloAleatorio" para que esta pueda ser utilizada dentro del vector de threads

    while (hiloAleatorio < 0 || hiloAleatorio > 14) // Se comprueba que el hilo seleccionado se encuentre dentro de los 15 existentes
    {
        cout << "El hilo no existe, el numero de hilo deberá ser entre el 0 y el 15. Ingrese el nuevo hilo: "; // En caso de que no exista se solicita un nuevo ingreso.
        cin >> hiloAleatorio;
        cout << "\n\n";  // Se almacena nuevamente el número de hilo
        hiloAleatorio--; // Se vuelve a ajustar el valor del parámetro para poder ser utilizado en el vector de threads
    }

    cout << "Hilo pausado aleatoriamente: " << hiloAleatorio + 1 << "\n";
    // cout << "Posicion del vector de hilos donde se encuentra el hilo aleatorio: " << hiloAleatorio << "\n\n";

    // Comienzo de la creación de cada uno de los threads en el vector de threads P
    for (int i = 0; i < 15; i++)
    {
        textoM = texto;             // Se asigna el valor de texto a textoM
        textoM += to_string(i + 1); // Se agrega el número de hilo a la cadena texto

        // cout << "\nSe crea el hilo " << textoM << "\n";
        dA = demoraAleatoria(); // Se genera la demora aleatoria
        // cout << "Demora Aleatoria: " << dA << "\n";
        vA = vecesAleatoria(); // Se genera las repeticiones aleatorias
        // cout << "Veces Aleatorias: " << vA << "\n";

        // La demora de los hilos estándar estará dada por la siguiente ecuación: (número de posición de hilo + 1) * 5
        // Hilo 0 = demora 5; Hilo 1 = demora 10; etc.
        dE = (i + 1) * 5; // Se calcula la demora estándar del hilo
        // cout << "Demora Estandar: " << dE << "\n";

        // Se identifica si el hilo a crear es el seleccionado para pausarse de forma aleatoria
        if (i != hiloAleatorio)
        {
            P[i] = thread(&misaludo, textoM, dA, vA); // Se crea el hilo con cadena especifica, demora y numero de ejecuciones aleatorias
        }
        else
        {
            P[i] = thread(&misaludo, textoM, dE, vA); // Se crea el hilo con los valores específicos de cadena, demora estándar y numero de ejecuciones aleatoria
        }
    }
    // Fin de la creación de los threads en el vector de treads P

    // Inicio de los threads
    for (int i = 0; i < 15; i++)
    {
        P[i].join();
    }
    // Fin de los threads

    cout << "\nFin \n"; // Finaliza la ejecución mostrando la cadena "Fin" por la terminal
}