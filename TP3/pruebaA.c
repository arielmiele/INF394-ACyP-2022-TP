#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int numJugadores = 4;
    int numPipes = numJugadores * 2;
    int fd[numPipes][2];
    int i;
    for (i = 0; i < numPipes; i++)
    {
        if (pipe(fd[i]) < 0)
        {
            printf("Problema abriendos los pipes.\n");
            return 1;
        }
    }

    int pid1 = fork();
    if (pid1 < 0)
    {
        printf("Problema creando el fork.\n");
        return 2;
    }

    if (pid1 == 0)
    {
        // HIJO JUGADOR
    }

    return 0;
}