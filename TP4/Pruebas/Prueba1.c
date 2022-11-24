#include <mpi.h>

int main(int argc, char const *argv[])
{
    int k; // Rango del proceso
    int p; // Numero del proceso
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &k);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    printf("Soy el proceso %d de %d\n", k, p);
    MPI_Finalize;
    return 0;
}
