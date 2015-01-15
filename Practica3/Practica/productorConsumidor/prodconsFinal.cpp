#include "mpi.h"
#include <iostream>
#include "math.h"
#include <time.h>
#include <stdlib.h>


//#define Productor 0
#define Buffer  5
//#define Consumidor 2
#define ITERS 20
#define TAM 5

using namespace std;

void imprimeBuffer( int * buffer, int pos){
  cout << " ## Buffer ## (pos: " << pos << ") ";

for(int i=0; i<TAM; i++){
  //Cuando hay datos, como pos marca la última celda con ellos se imprimen hasta ella.
  if(i<=pos-1)
    cout << " [ " << buffer[i] << " ] ";
  else //Se imprime el resto de celdas vacías:
    cout << " [ ] ";
}
cout << endl;

/*
  for(int i=0; i<=pos; i++)
    cout << " [ " << buffer[i] << " ] ";
  for(int i=pos; i<TAM-1; i++)
    cout << " [ ] ";
  cout << endl;
*/

}

void productor(int numeroProductor){


  int value;
  //Número de veces que el productor produce: 20(ITERS)
  for (unsigned int i=0;i<ITERS;i++){
    value=i;

    // cout<< "Productor " << numeroProductor << " produce " <<value<<endl<<flush;
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

  //Si el productor se ejecuta 20 veces y el consumidor otras 20 el buffer tendrá que hacer el doble de operaciones, ya que interviene en todas. 9(operaciones) x 20
  for (unsigned int i=0;i<ITERS*9;i++){
    cout << "Iteración : " << i << endl;

    /*Si el buffer está vacío el indice "pos" estará a 0 y por tanto no se puede leer de el buffer, sólo escribir. */
    if (pos==0){
      cout << "Selección de rama 0. Buffer vacío. " << endl;
      rama=0;
      /* Si el bufer está lleno "pos" será igual al tamaño del buffer, habrá sobrepasado la última posición de escritura del buffer y
      ya no se podrá escribir, sólo leer de el */
      }else if (pos==TAM){
      cout << "Selección de rama 1. Buffer lleno. " << endl;
      rama=1;
      /*Cuando el índice no esté ni a 0 (buffer vacío) ni a TAM (buffer lleno) estará en una posicón válida de escritura. */
      }else{
        //Chequeo bloqueante para un mensaje.
        //Consulto un mensaje que me llega bloqueándolo, para tomar en este caso: UNA DECISION ANTES :> QUIEN LO RESOLVERÁ
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout << "Analizando mensaje de proceso " << status.MPI_SOURCE << endl;
        //Cuando el mensaje llegue (puedes venirle del productor o del consumidor) activa una u otra acción.

        //En este caso no solo hay un productor y hay que poder reconocer un mensaje enviado por cualquier de los que existen.
        if (status.MPI_SOURCE==0 || status.MPI_SOURCE==1 || status.MPI_SOURCE==2 || status.MPI_SOURCE==3 || status.MPI_SOURCE==4)
          rama=0; //Se trata de un mensaje de un productor y activamos la acción necesaria en el buffer.
          else //(serían los procesos del 6 al 9)
          rama=1; //Se trata de un proceso consumidor.
      }

          cout << "SWITCH rama " << endl;

          switch(rama){
            //Acción 0 || Productor ||
            //Si recibe un mensaje del productor es porque le está enviando datos.
            case 0:
            //Recibe los datos del productor y los graba en la posición del buffer indicada por pos &value[pos]

            /*
            En este caso necesitamos recibir los mensajes de los productores para grabar en el buffer pero para ello debemos identificar
            el productor desde el que nos viene, para eso podemos pensar en añadir varias funciones, tantas como productores, o
            usar un tag distinto para unos y otros.
            */

            //Recibimos de cualquier productor:
            MPI_Recv( &value[pos], 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status);
            cout<< "Buffer recibe "<< value[pos] << " de Productor " << status.MPI_SOURCE <<endl<<flush;

            //Como esa posición ha sido utilizada se adelanta pos una unidad.
            pos++;
            imprimeBuffer(value, pos);
            break;
            //Acción 1 || Consumidor ||
            //Si recibe un mensaje del consumidor es que quiere que le envíe un dato.
            case 1:
            //Se recibe la peticion sin hacer nada con ella (con el dato que nos envía "peticion") del consumidor identificado con MPI_SOURCE
            MPI_Recv( &peticion, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status);
            /*Se le envía el dato de forma síncrona (de la posicon pos-1 porque  pos ya ha sido movido antes) para que quede bloqueado el proceso hasta que el
            receptor empieza a reicibir la información.*/
            //Se envía el dato al consumidor que ha enviado el mensaje y que nosotros antes hemos captado su identificado con MPI_Probe
            MPI_Ssend( &value[pos-1], 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            cout<< "Buffer envía "<< value[pos-1] << " a Consumidor "<<endl<<flush;
            //Como el dato ha sido enviado y esa celda "se vacía (no lo hace realmente)" se atrasa el índice una posición.
            pos--;
            imprimeBuffer(value, pos);

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
      //    cout<< "Consumidor recibe valor "<<value<<" de Buffer "<<endl<<flush;
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
        if (size!=10) {cout<< "El numero de procesos debe ser 10, 5 productores, 1 buffer y 4 consumidores " <<endl;return 0;}
          if ( rank==0 || rank==1 || rank==2 || rank==3 || rank==4 ){
            //cout << "Se lanza productor " << rank << endl;
            productor(rank);
          }else if (rank==Buffer){
              //cout << "Se lanza el buffer";
              buffer();
            }else {
                //cout << "Se lanza consumidor " << rank << endl;
                consumidor();
              }


            MPI_Finalize( );
            return 0;
          }


/*
Si los mensajes por terminal se añaden en todos los procesos podemos tener errores en el orden, por eso es mejor sólo imprimirlos en el proceso buffer
que sabemos que sólo es uno.
*/
