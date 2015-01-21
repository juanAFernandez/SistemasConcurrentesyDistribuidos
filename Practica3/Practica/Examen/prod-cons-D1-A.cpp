#include "mpi.h"
#include <cmath>
#include <iostream>
#define etiq_consumidor 1    // Etiqueta para mensajes de los consumidores
#define etiq_productor 2     // Etiqueta para mensajes de los productores
#define TAM 5		// Tamaño del buffer
#define NUM_ELEM 20	// Número total de elementos
#define num_productores 5  // Número de procesos productores
#define num_consumidores 4 // Número de procesos consumidores

using namespace std;

int Buffer;  // Identificador del proceso Buffer (se calcula dependiendo del número de productores)

void productor(int id){
        int iters, val;
        iters= NUM_ELEM/num_productores;
	for (unsigned int i=0;i<iters;i++){
                val= id*iters + i;
		cout<< "Productor "<<id<< " produce valor "<<val<<endl<<flush;
		MPI_Ssend( &val,1, MPI_INT, Buffer, etiq_productor, MPI_COMM_WORLD );
	}
}
void consumidor(int id){
	int val,iters,peticion=1; float raiz;
	MPI_Status status;
        iters= NUM_ELEM/num_consumidores;
	for (unsigned int i=0;i<iters;i++){
		MPI_Ssend(&peticion,1, MPI_INT, Buffer, etiq_consumidor,MPI_COMM_WORLD);
		MPI_Recv(&val,1, MPI_INT, Buffer, etiq_consumidor, MPI_COMM_WORLD,&status );
		cout<< "Consumidor " <<id << " recibe valor "<<val<<" de Buffer "<<endl<<flush;
		raiz=sqrt(val);
	}
}
void buffer(){
	int value[TAM], peticion, pos=0,rama; MPI_Status status;
	for (unsigned int i=0;i<NUM_ELEM*2;i++){
		if (pos==0)
			rama=0; //El consumidor no puede consumir, se puede producir porque el vector está vacío.
		else if (pos==TAM)
			rama=1; // El productor no puede producir. No se puede producir porque el vector esta lleno, sólo se puede leer.
		else{ //Ambos pueden actuar
			MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&status);
		        if (status.MPI_TAG==etiq_productor) //Si la etiqueta es la de productor se procesa por la rama de productor.
			    rama =0;
		        else //Se trata de un consumidor y se pasa a la rama 1
			    rama=1;
		   }
		switch(rama){
		case 0: //Rama de atención al productor.
			MPI_Recv(&value[pos],1,MPI_INT, MPI_ANY_SOURCE,etiq_productor,MPI_COMM_WORLD,&status);
			cout<< "Buffer recibe "<< value[pos] << " de Prod "<<status.MPI_SOURCE<<endl<<flush;
			pos++;
			break;
		case 1: //Rama de atención del consumidor
			MPI_Recv(&peticion,1,MPI_INT,MPI_ANY_SOURCE,etiq_consumidor,MPI_COMM_WORLD,&status);
			cout<< "Buffer envia "<< value[pos-1] << " a Cons."<<status.MPI_SOURCE<<endl<<flush;
			MPI_Ssend(&value[pos-1],1,MPI_INT,status.MPI_SOURCE, etiq_consumidor, MPI_COMM_WORLD);
			pos--;
		break;
    }
	}
}



int main(int argc, char *argv[]) {
	int rank,size, total;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
        total= num_productores + num_consumidores + 1;
        Buffer= num_productores;
	if( size != total)
	    {
	      if (rank == 0)
		   cout<<"El numero de procesos debe ser num_productores + num_consumidores + 1"<<endl<<flush;
	      MPI_Finalize( );
	      return 0;
	    }
	if (rank <num_productores) productor( rank);
	else if (rank==Buffer) buffer();
	else consumidor(rank);
	MPI_Finalize( );
	return 0;
}
