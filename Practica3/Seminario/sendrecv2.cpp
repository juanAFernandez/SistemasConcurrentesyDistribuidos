#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {

  int rank, size,value;
  MPI_Status status;

 //Inicio igual que siempre:
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size( MPI_COMM_WORLD, &size );


 do {
  //Si el proceso es el número 0 de todos los que hay en el comunicador:
  if (rank == 0) {
    //Lee datos desde la entrada estandar:
    scanf( "%d", &value );
    //Envía el dato introducido al proceso siguiente (0+1=1)
    MPI_Send( &value, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD );

  //Cuando no sea el proceso 0 (el que envió el proceso original):
  }else{

     //Queda esperando un mensaje del proceso igual a el menos 1
     MPI_Recv( &value, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status );


     if (rank < size-1)
        //Si el proceso no es el último envía el mensaje recibido al siguiente:
        MPI_Send( &value, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD );
  }
  cout<< "Soy el proceso "<<rank<<" y he recibido "<<value<<endl;

 } while (value >= 0);

 MPI_Finalize(); return 0;
}
