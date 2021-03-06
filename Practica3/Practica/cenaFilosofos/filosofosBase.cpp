#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

void Filosofo(int id, int nprocesos);
void Tenedor (int id, int nprocesos);

int main(int argc,char** argv ){
 int rank,size;
 srand(time(0));
 MPI_Init( &argc, &argv );
 MPI_Comm_rank( MPI_COMM_WORLD, &rank );
 MPI_Comm_size( MPI_COMM_WORLD, &size );
 if( size!=10){
   if(rank == 0) 
      cout<<"El numero de procesos debe ser 10"<<endl<<flush;
   MPI_Finalize( ); return 0; 
 }
 if ((rank%2) == 0)  
      Filosofo(rank,size); // Los pares son Filosofos 
 else Tenedor(rank,size);  // Los impares son Tenedores 
 MPI_Finalize( );
 return 0;
}  

void Filosofo(int id, int nprocesos){
 int izq=(id+1)%nprocesos;int der=(id-1+nprocesos)%nprocesos;   
 while(1){
  //Solicita tenedor izquierdo
  cout<<"Filosofo "<<id<< " solicita tenedor izq ..."<<izq <<endl<<flush;
  // ...
  //Solicita tenedor derecho
  cout<<"Filosofo "<<id<< " coge tenedor der ..."<<der <<endl<<flush;
  // ...
  cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
  sleep((rand() % 3)+1);  //comiendo
  //suelta el tenedor izquierdo
  cout<<"Filosofo "<<id<< " suelta tenedor izq ..."<<izq <<endl<<flush;
  // ...
  //suelta el tenedor derecho
  cout<<"Filosofo "<<id<< " suelta tenedor der ..."<<der <<endl<<flush;
  // ...
  cout<<"Filosofo "<<id<< " PENSANDO"<<endl<<flush;
  sleep((rand()%3)+1 );//pensando
 }
}

void Tenedor(int id, int nprocesos){
  int buf; MPI_Status status; int Filo;
  while(1){
    // Espera un peticion desde cualquier filosofo vecino ...
    // ...
    // Recibe la peticion del filosofo ...
    // ...
    cout<<"Ten. "<<id<<" recibe petic. de "<<Filo<<endl<<flush;
    // Espera a que el filosofo suelte el tenedor...
    // ...
    cout<<"Ten. "<<id<<" recibe liberac. de "<<Filo<<endl<<flush; 
  }
}