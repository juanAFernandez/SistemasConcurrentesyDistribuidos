#include "mpi.h"
#include <iostream>
#include "math.h"
#include <time.h>
#include <stdlib.h>


#define Productor 0
#define Buffer  1
#define Consumidor 2
#define ITERS 20
#define TAM 5

using namespace std;

void productor(){
int value;
//Número de veces que el productor produce: 20(ITERS)
for (unsigned int i=0;i<ITERS;i++){
  value=i;
  cout<< "Productor produce valor "<<value<<endl<<flush;
  sleep(rand() % 2 );
  //Envío del contenido de la variable value, que es una unidad, de tipo MPI_INT al proceso Buffer (1),
  //con la etiqueta 0 en el comunicador MPI_COMM_WORLD
  MPI_Ssend( &value, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD );
 }
}


void buffer(){

 int value[TAM]; //Vector que representa el buffer de tamaño TAM=5
 int pos=0; //Indice  para moverse por el vector

 int peticion;

 int rama;

 MPI_Status status; //Necesario para las funciones Recv y Probe.

 //Si el productor se ejecuta 20 veces y el consumidor otras 20 el buffer tendrá que hacer el doble de operaciones, ya que interviene en todas.
 for (unsigned int i=0;i<ITERS*2;i++){

   /*Si el buffer está vacío el indice "pos" estará a 0 y por tanto no se puede leer de el buffer, sólo escribir. */
   if (pos==0)
     rama=0;
   /* Si el bufer está lleno "pos" será igual al tamaño del buffer, habrá sobrepasado la última posición de escritura del buffer y
   ya no se podrá escribir, sólo leer de el */
   else if (pos==TAM)
        rama=1;

        /*Cuando el índice no esté ni a 0 (buffer vacío) ni a TAM (buffer lleno) estará en una posicón válida de escritura. */
        else{
          //Chequeo bloqueante para un mensaje.
          // Con esto lo que se hace es bloquear al proceso hasta que por el comunicador llegue algún mensaje
          MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          //Cuando el mensaje llegue (puedes venirle del productor o del consumidor) activa una u otra acción.
          if (status.MPI_SOURCE==Productor)
            rama =0;
          else
            rama=1;
        }

   switch(rama){
    //Acción 0 || Productor ||
    //Si recibe un mensaje del productor es porque le está enviando datos.
     case 0:
       //Recibe los datos del productor y los graba en la posición del buffer indicada por pos &value[pos]
       MPI_Recv( &value[pos], 1, MPI_INT, Productor, 0, MPI_COMM_WORLD,&status);
       cout<< "Buffer recibe "<< value[pos] << " de Productor "<<endl<<flush;
       //Como esa posición ha sido utilizada se adelanta pos una unidad.
       pos++;
       break;
    //Acción 1 || Consumidor ||
    //Si recibe un mensaje del consumidor es que quiere que le envíe un dato.
     case 1:
       //Se recibe la peticion sin hacer nada con ella (con el dato que nos envía "peticion").
       MPI_Recv( &peticion, 1, MPI_INT, Consumidor, 0, MPI_COMM_WORLD,&status);
       /*Se le envía el dato de forma síncrona (de la posicon pos-1 porque pos ya ha sido movido antes) para que quede bloqueado el proceso hasta que el
       receptor empieza a reicibir la información.*/
       MPI_Ssend( &value[pos-1], 1, MPI_INT, Consumidor, 0, MPI_COMM_WORLD);
       cout<< "Buffer envía "<< value[pos-1] << " a Consumidor "<<endl<<flush;
       //Como el dato ha sido enviado y esa celda "se vacía (no lo hace realmente)" se atrasa el índice una posición.
       pos--;
       break;
   }

 }

}


void consumidor(){
 int value,peticion=1; float raiz;
 MPI_Status status;
 for (unsigned int i=0;i<ITERS;i++){
  MPI_Ssend(&peticion, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD);
  MPI_Recv(&value, 1,     MPI_INT, Buffer, 0, MPI_COMM_WORLD,&status );
  cout<< "Consumidor recibe valor "<<value<<" de Buffer "<<endl<<flush;
  sleep(rand() % 2 );
  raiz=sqrt(value);
 }
}


int main(int argc, char *argv[]) {
  int rank,size;
  cout.flush();
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  /* Inicializa la semilla aleatoria */
  srand ( time(NULL) );

  //Decisión por rank del método al que llamaŕa cada proceso. 
  if (size!=3)
   {cout<< "El numero de procesos debe ser 3 "<<endl;return 0;}
  if (rank == Productor)
    productor();
  else if (rank==Buffer)
    buffer();
  else consumidor();


  MPI_Finalize( );
  return 0;
}
