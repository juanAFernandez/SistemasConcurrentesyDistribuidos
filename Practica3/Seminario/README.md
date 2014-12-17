#Seminario

###Hola mundo

Como siempre el primer programa que debemos ver para entender la tecnología será el hola mundo. En este caso este simple programa hace que cada proceso muestre un mensaje en el que dice quien es del número total de procesos. El identificador unívoco del proceso es **rank** y el número total de procesos **size**

~~~c
#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

  //Siempre necesitaremos estas variables:
  int rank, size;

  //Estas tres sentencias son comunes para todos los programas que hagamos:
  MPI_Init(&argc, &argv); //Inicializa el entorno de ejecución de MPI
  MPI_Comm_size(MPI_COMM_WORLD, &size); //Determina el número de procesos de un comunicador
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Determina el identificador del proceso en un comunicador.

  cout<<"Hola desde proc. "<<rank<<" de "<<size<<endl;

  //Siempre debemos de finalizar el entorno de ejecución de MPI.
  MPI_Finalize();

  return 0;
}
~~~

**MPI_COMM_WORLD** hace referencia al **comunicador universal**, predefinidor por MPI que incluy a todos los procesos de la aplicación (comunicador por defecto).

Salida de una ejecución:

~~~bash
Ubuntu> mpirun -np 4 holamundo
Hola desde proc. 0 de 4
Hola desde proc. 1 de 4
Hola desde proc. 2 de 4
Hola desde proc. 3 de 4
~~~


###Envío y recepción simple

~~~ c
#include "mpi.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){

  int rank, size,value;
  MPI_Status status;

  //Las tres siguientes órdenes son exactamente igual en todos los programas:
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if (rank == 0) {
    value=100;
    MPI_Send (&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD );
    cout<<"Proceso "<<rank<< " ha enviado "<<value<< " al proceso 1"<<endl;
    }else{
      MPI_Recv ( &value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
      cout<<"Proceso "<<rank<< " ha recibido "<<value<< " de proceso 0"<<endl;
    }

    MPI_Finalize( );
    return 0;

  }
~~~








###Compilación y ejecución.

Para compilar un programa con OpenMPI necesitamos usar **script específicos** :

  * Para compilar: **mpicxx**
  * Para ejecutar: **mpirun**

Ejemplo:

    mpicxx -o holamundo holamundo.cpp

    mpirun -np 4 holamundo

    //Donde con np especificamos el número de procesos que queremos que ejecuten el código.
