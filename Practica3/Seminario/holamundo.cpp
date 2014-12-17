#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

    int rank, size;

    //Estas tres sentencias son comunes para todos los programas que hagamos:
    MPI_Init(&argc, &argv); //Inicializa el entorno de ejecución de MPI
    MPI_Comm_size(MPI_COMM_WORLD, &size); //Determina el número de procesos de un comunicador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Determina el identificador del proceso en un comunicador.


    cout<<"Hola desde proc. "<<rank<<" de "<<size<<endl;

    MPI_Finalize();

    return 0;
}
