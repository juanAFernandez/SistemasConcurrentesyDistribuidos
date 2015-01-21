/*
  @Author: Juan Antonio Fernández Sánchez
  @email: juan.antonio.fernandez.sanchez.88@gmail.com
  @date: 20 Enero 2015

  @About:
  Implementación para examenA donde cada filósofo sólo puede sentarse un determinado número de veces,
  como si sólo pudiera gastar un determinado número de tickets comiendo. Cuando este número de tickets se le acaba
  deja de pedirle al camarero que quiere sentarse y se se va ("mostrando un mensaje"). Cuando esto le sucede a todos
  filósofos el prgrama queda en estado bloqueado.

  @Execute: mpicxx -o filosofosConCamareroExamenA filosofosConCamareroExamenA.cpp ; mpirun -np 11 filosofosConCamareroExamenA
*/


#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define TAG_FILOSOFOS 0

#define MAX_TICKET 2

#define CAMARERO 10

using namespace std;

void Filosofo(int idProcesoTenedor, int nprocesos);
void Tenedor (int id, int nprocesos);
void Camarero (int idCamarero, int nprocesos);

int main(int argc,char** argv ){
  int rank,size;
  srand(time(0));

  //Inicialización general de todo programa MPI
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if( size!=11){
    if(rank == 0)
      cout<<"El numero de procesos debe ser 11"<<endl<<flush;
      MPI_Finalize( ); return 0;
  }

  //Selección del rol aisgnado a cada proceso.
  if( rank==10  )
      Camarero(rank, size); //El proceso n.11 (id/rank=10) será el que ejecute el método camarero.
  else if ((rank%2) == 0)
        Filosofo(rank,size-1); // Los pares son Filosofos
       else Tenedor(rank,size-1);  // Los impares son Tenedores

    /* Como hemos añadido un nuevo proceso, para que los filósofos una vez en la mesa y los tenedors funcionando
       sigan comportándose como en la primera versión enviamos el número 10 como size para que no alteren su funcionamiento.
    */


    MPI_Finalize( );
    return 0;
    }


    //Cada filósofo debe pedir permiso al camarero para sentarse.
    //El camarero permitirá sentarse como máximo a 4 filósofos en la mesa.
    void Camarero(int idCamarero, int nprocesos){

      MPI_Status status;

      cout << "##CAMARERO## Empieza a trabajar!" << endl << flush;


      /*El camarero debe poder gestionar a los comensales y no permitir que se sientes más de cuatro.
      Cada filósofo tiene su sitio asignado por lo que no tiene que preocuparse por eso.
      Para realizar esta gestión podemos declarar un avariable que sea filosofosSentados y que aumente o
      disminuya conforme vayan sentándose.

       */
      int filosofosSentados=0;
      int peticion=0, respuesta=0;
      bool sentarse=false;
      bool levantarse=false;

      while(1){//Me mantengo a la escucha constantemente


        //1. Recibio petición de un filósofo que quiere hacer algo:

          MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        //2. Compruebo de que tipo es la petición, si de sentarse o de levantars de la mesa

          if(peticion==1){ //Quiere sentarse
            sentarse=true;
            // cout << "Camarero recibe petición del proces " << status.MPI_SOURCE << " que quiere sentarse. " << endl << flush;
          }
          if(peticion==0){ //Quiere levantarse
            levantarse=true;
            // cout << "Camarero recibe petición del proces " << status.MPI_SOURCE << " que quiere levantarse. " << endl << flush;
          }


        if(sentarse){
          // cout << "Camarero revista el estado de la mesa " << endl << flush;
        //3. Compruebo si hay sentados cuatro filósofos:

          if(filosofosSentados<4){
            cout << "Camarero dice: hay " << filosofosSentados << " filosofos sentados " << endl << flush;
            cout << "Camarero sienta a proceso  " << status.MPI_SOURCE << " en su sitio " << endl << flush;
            respuesta=1; //RESPUESTA POSITIVA->> puede sentarse!
            filosofosSentados++; //Aumento porque uno se va a sentar
          }else{
            //cout << "Camarero dice: no puede sentarse, hay " << filosofosSentados  << " filósofos " << endl << flush;
            respuesta=0; //RESPUESTA NEGATIVA->> NO puede sentarse!
          }
        //4. Respondo al filósofo que me ha enviado la petición sin importarme quien ha sido:

          //Le respondo, respuesta=1 si puede sentarse e =0 si no puede.
          MPI_Ssend(&respuesta, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

          sentarse=false; //reinicio
        }


        if(levantarse){

          /*Si quiere levantarse no se lo impido. */

          //3. Respondo
          MPI_Send(&respuesta, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
          filosofosSentados--;

          levantarse=false; //reinicio
        }



     }//fin while



    }//fin Camarero.


    void Filosofo(int id, int nprocesos){


      int numTicketGastados=0;


      int numFilosofo;
      if(id==0) numFilosofo=0;
      if(id==2) numFilosofo=1;
      if(id==4) numFilosofo=2;
      if(id==6) numFilosofo=3;
      if(id==8) numFilosofo=4;



      /*Conociendo la distribución de los comensales y los tenedores en la mesa,
      podemos saber cual es el tenedor que tengo a mi derecha y a mi izquierda. Necesito
      identificarlos para poder enviarles mensajes de forma correcta*/

      int izq=(id+1)%nprocesos; //Proceso de mi izquierda
      int der=(id-1+nprocesos)%nprocesos; //Proceso de mi derecha


    int peticion=0, respuesta=0;
    MPI_Status status;
    bool puedoSentarme=false;
    bool puedoLevantarme=false;

    while(numTicketGastados<=2){
      //Bucle de petición de permiso de sentarme:
      while(puedoSentarme==false){

      // ##SECCIÓN INICIAL CAMARERO## //

      //0. Antes de nada pide permiso para sentarse al camarero:

      //Envío la petición al camarero.
      peticion=1;//Significa que me quiero sentar.
      MPI_Ssend(&peticion, 1, MPI_INT, CAMARERO, 0, MPI_COMM_WORLD );


      //Me quedo esperando la respuesta del camarero para continuar:
      MPI_Recv(&respuesta, 1, MPI_INT, CAMARERO, 0, MPI_COMM_WORLD, &status);
      if(respuesta==1){//Puedo sentarme
        puedoSentarme=true; //Salgo del bucle de petición de permiso.
      //una vez recibida puedo empezar a comer.
        cout << "Filosofo " << numFilosofo << " se sienta a la mesa" << endl;
      }
      // ##FIN SECCIÓN INICIAL CAMARERO## //

      }

      //1. Necesita comer, para eso necesita dos tendores, los pido:

      // ## PETICIÓN TENEDOR IZQUIERDO ##
      cout << "Filosofo " << numFilosofo << " solicita tenedor izq, proceso y tag: " << izq << endl << flush;
      /*Envío una petición al tenedor de mi izquierda, especificando destino: proceso izquierda y usando
      el mismo valor para tag para que śolo ese proceso tenedor reciba este mensaje de mi parte,
      ya que puede recibir tb de otro fiĺosofo que esté a su lado.*/
      MPI_Ssend(&peticion, 1, MPI_INT, izq, izq, MPI_COMM_WORLD);

      // ## PETICIÓN TENEDOR DERECHO ##
      cout << "Filosofo " << numFilosofo << " solicita tenedor der, proceso: " << der << endl << flush;
      MPI_Ssend(&peticion, 1, MPI_INT, der, der, MPI_COMM_WORLD);


      //2.Me quedo esperando que los dos tenedores se queden libres y me respondan dandome permiso para usarlos:

      //Tenedor de la izquierda me da permiso de uso:
      cout << "FIlosofo: " << numFilosofo <<  "Esperando respuesta de uso del tenedor de la izquierda, proc " << izq << endl << flush;
      MPI_Recv(&respuesta, 1, MPI_INT, izq, izq, MPI_COMM_WORLD, &status);
      cout << "RECIBIDO TENEDOR de IZQUIERDA" << endl << flush;

      //Tenedor de la derecha me da permiso de uso:
      cout << "FIlosofo: " << numFilosofo <<  "Esperando respuesta de uso del tenedor de la derecha, proc " << izq << endl<<  flush;
      MPI_Recv(&respuesta, 1, MPI_INT, der, der, MPI_COMM_WORLD, &status);
      cout << "RECIBIDO TENEDOR de DERECHA" << endl << flush;


      //3.Con ambos tenedores en mi poder me pongo a comer:

      // ## COMIENDO ## //
      //Cuando ha recibido los tenedores come:
      cout << "Filosofo " << numFilosofo << " ## COMIENDO ## " << endl << flush;
      sleep((rand() % 10)+1);  //comiendo
      // ## FIN COMER ## //

      /*4.Cuando termino de comer dejo de usar los tenedores enviandoles un mensaje para que se
      desbloqueen de mi*/

      //suelta el tenedor izquierdo
      cout<<"Filosofo "<<id<< " suelta tenedor izq ..."<<izq <<endl<<flush;
      MPI_Ssend(&peticion, 1, MPI_INT, izq, izq, MPI_COMM_WORLD);


      //suelta el tenedor derecho
      cout<<"Filosofo "<<id<< " suelta tenedor der ..."<<der <<endl<<flush;
      MPI_Ssend(&peticion, 1, MPI_INT, der, der, MPI_COMM_WORLD);


      //Bucle de petición de permiso de levantarme:
      while(puedoLevantarme==false){

        // ##SECCIÓN FINAL CAMARERO## //

        //Para terminal pido permiso al camarero para levantarme:

        //Envío la petición al camarero.
        peticion=0;//Significa que me quiero LEVANTAR.
        MPI_Ssend(&peticion, 1, MPI_INT, CAMARERO, 0, MPI_COMM_WORLD );


        //Me quedo esperando la respuesta del camarero para continuar:
        MPI_Recv(&respuesta, 1, MPI_INT, CAMARERO, 0, MPI_COMM_WORLD, &status);
        if(respuesta==1)//Puedo levantarme
          puedoLevantarme=true; //Salgo del bucle de petición de permiso.

          // ##FIN SECCIÓN FINAL CAMARERO## //
      }

      numTicketGastados++;

      //5.Después del permiso del camarero para retirarme y después de soltar los tenedores me pongo a pensar un rato, fuera de la mesa:

      cout<<"Filosofo "<<id<< " PENSANDO"<<endl<<flush;
      sleep((rand()%10)+1 );//pensando


    }

    //Ya no le quedan más ticket para comer:

    cout << "Filósofo " << numFilosofo << " no me quedan tickets. Me voy! " << endl << flush;

  }

  void Tenedor(int idProcesoTenedor, int nprocesos){

    /*Este método se ejecuta por cinco procesos distintos y para todos deber servir el mismo código que implementa.*/

    //Diferenciación entre número de proceso y el número de tenedor, útil para los mensajes por pantalla:
    int numTenedor;
    if(idProcesoTenedor==1) numTenedor=0;
    if(idProcesoTenedor==3) numTenedor=1;
    if(idProcesoTenedor==5) numTenedor=2;
    if(idProcesoTenedor==7) numTenedor=3;
    if(idProcesoTenedor==9) numTenedor=4;

    //Según el tenedor que seamos, tendremos a nuestra derecha e izquierda unos filósofos u otros, los especificamos:
    int filosofoIzquierda=(numTenedor+1)%5, filosofoDerecha=numTenedor;

    //Identificación de los números de proceso de los procesos colindantes:
    int izq=(idProcesoTenedor+1)%nprocesos; //Proceso de mi izquierda
    int der=(idProcesoTenedor-1+nprocesos)%nprocesos; //Proceso de mi derecha

    //Mostramos por terminal nuestra situación:
    cout << "Tenedor n. " << numTenedor << " con filosofos " << filosofoDerecha << " a la derecha y " << filosofoIzquierda << " a la izquierda. " << endl << flush;

    int buf; MPI_Status status; int Filo; int peticion, respuesta;
    while(1){
      /* Espera un peticion desde cualquier filosofo vecino, por eso MPI_ANY_SOURCE, pero sólo puede recibir de los dos que
      tiene a su lado, de ninguno más, porque sólo los filósofos que tiene a su lado usan el tag de este tenedor.*/
      MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, /*tag*/ idProcesoTenedor, MPI_COMM_WORLD, &status);
      // Recibe la peticion del filosofo ...


      /*Cuando recibe el mensaje, no sabemos a quien enviarle la respuesta, examinamos MPI_SOURCE de status para
      saber de que filósofo nos ha venido la petición y ser a el y no a otro a quién darle nuestro uso.*/

      if(status.MPI_SOURCE==der){ //Petición del filósofo de la derecha (selección por el número de proceso)
        cout << "Ten. " << numTenedor << " recibe petic. de filósofo " << filosofoDerecha << " proces: " << (idProcesoTenedor-1)%nprocesos << endl << flush;
        //Envio mensaje al filósofo para que me use, send
        //Send.
        cout << "Ten " << numTenedor << " Envío a filosofo: " << filosofoDerecha << " num proc. " << der << " con tag " << idProcesoTenedor <<  endl << flush;
        MPI_Ssend(&respuesta, 1, MPI_INT, der, idProcesoTenedor, MPI_COMM_WORLD);
        //Cuando hago el envío de "mi uso" tengo que quedarme esperando para que me desbloqueen, con un Recv bloqueante:
        cout << "Ten " << numTenedor << " queda esperando liberación de  " << filosofoDerecha << endl << flush;
        //RECV, para quedarme esperando a que me libere.
        MPI_Recv(&peticion, 1, MPI_INT, der,idProcesoTenedor, MPI_COMM_WORLD, &status);

        cout << "Ten. " << idProcesoTenedor << " recibe liberac. de " << filosofoDerecha << endl << flush;
      }

      if(status.MPI_SOURCE==izq){ //Petición del filósofo de la izquierda (selección por el número de proceso)
        cout << "Ten. " << numTenedor << " recibe petic. de filósofo " << filosofoIzquierda << " proces: " << (idProcesoTenedor+1)%nprocesos << endl << flush;
        //Envio mensaje al filósofo para que me use, send
        //Send.
        cout << "Ten " << numTenedor << " Envío a filosofo: " << filosofoIzquierda << " num proc. " << izq << " con tag " << idProcesoTenedor <<  endl << flush;
        MPI_Ssend(&peticion, 1, MPI_INT, izq, idProcesoTenedor, MPI_COMM_WORLD);
        //Cuando
        //RECV, para quedarme esperando a que me libere.
        MPI_Recv(&peticion, 1, MPI_INT, izq, idProcesoTenedor, MPI_COMM_WORLD, &status);

        cout << "Ten. " << idProcesoTenedor << " recibe liberac. de " << filosofoIzquierda << endl << flush;
      }



    }
  }
