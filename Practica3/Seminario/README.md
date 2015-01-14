#Seminario: Introduccióna MPI

###Hola mundo

Como siempre el primer programa que debemos ver para entender la tecnología será el "hola mundo". En este caso este simple programa hace que cada proceso muestre un mensaje en el que dice quien es del número total de procesos. El identificador unívoco del proceso es **rank** y el número total de procesos **size**

~~~c
// holamundo.cpp
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

**MPI_COMM_WORLD** hace referencia al **comunicador universal**, predefinidor por MPI que incluye a todos los procesos de la aplicación (comunicador por defecto). La identificación (rank) de los procesos participantes en un comunicador es unívoca de cada proceso, lo que no quiere decir que un proceso no pueda participar en varios comunicadores. Los mensajes que se envién a diferentes contextos de comunicación (con diferentes comunicadores) no se interfieren entres si.

Salida de una ejecución:

~~~bash
Ubuntu> mpirun -np 4 holamundo
Hola desde proc. 0 de 4
Hola desde proc. 1 de 4
Hola desde proc. 2 de 4
Hola desde proc. 3 de 4
~~~

Podemos ver ese **4** al imprimir *size* porque cada proceso que se incia con ese comunicador eleva el número de esa variable (que es local para cada proceso).

Las **6 operaciones básicas** que manejaremos con MPI serán entonces:

* **MPI_Init**: Para inicializar el entorno de ejecución de MPI. DEbe llamarse siempre antes de cualquier otra función de MPI. Si se llama más de una vez durante la ejecución dará un error. Los argumentos *argc* y *argv* son los argumentos de la línea de orden del programa.
* **MPI_FInalize**: Para finalizar el entorno de ejecución de MPI. Realiza las tareas de limpieza del entorno de ejecución.
* **MPI_Comm_size**: Para conocer el número de procesos que se están ejecutando
dentro de un comunicador.
* **MPI_Comm_rank**: Para conocer el identificador de un proceso en el entorno de un comunicador.

Las dos últimas son las que se encargar de realizar el envío y recepción de mensajes:

 **[MPI_Send](http://www.mpich.org/static/docs/v3.1/www3/MPI_Send.html)**: Para enviar mensajes de forma simple.
  Implementa envío **asíncrono seguro.** Su sintaxsis es:


    int MPI_Send(void *buf, int num, MPI_Datatype datatype, int dest, int tag, MPI_Comm)

Donde:

* *** buf**:  es la dirección de memoria (*0xbff32890* p.e.) donde se encuentran almacenados los datos que se van a enviar, por eso este parámetro debe pasarse con **&**.
* **num**: número de elementos en el buffer de envío (debe de ser un entero no negativo).
* **datatype**: es el tipo de dato que se va a envíar
* **dest**: destino, par aespecificar al proceso dentro del comunicador al que se va a enviar el mensaje.
* **tag**: sirve para poder especificar el proceso además de con el rank. Así sólo podran enviarse datos aquellos procesos que usen la misma tag y que además estén dentro del mismo comunicador.
* **comm**: especifica el comunicador por el que se hará el envío.





 **[MPI_Recv](http://www.mpich.org/static/docs/v3.1/www3/MPI_Recv.html)**: Para recibir mensajes de forma simple.

Su sintaxsis es:

    int MPI_Recv (void *buf, int num, MPI_Datatype datatype, int source, int tag, MPI_Comm com, MPI_Status *status);

Donde:

* *** buf**:  
* **num**: número de elementos en el buffer de recepción (debe de ser un entero no negativo).
* **datatype**: es el tipo de dato que se va a envíar
* **source**: identrificador del proceso que envió los datos.
* **tag**: etiqueta que nos sirve para que sólo los procesos emisor y receptor que tengan la misma puedan intercambiar información.
* **comm**: para especificar el comunicador usado.
* ** *status **: Se utiliza para guardar informarción sobre la operación de recepción de información . MPI_Status: Este tipo de dato guarda una estructura interna con los siguientes campos:
MPI_SOURCE	Indica el rango del proceso origen en el comunicador en el que se realizó la transmisión (int).
MPI_TAG	Valor de la etiqueta que tenía el mensaje (int).
size	Número de elementos del tipo de dato especificado en el mensaje.
count	Tamaño del mensaje en bytes (para obtener el número de elementos, por ejemplo si fuera un entero, sería "status.count/sizeof(int)").

###Envío y recepción simple

En el siguiente programa vemos como un proceso identificado por el **rank 0** dentro del entorno del comunicador **MPI_COMM_WORLD** envía un mensaje con contenido un entero.

~~~ c
// sendrecv1.cpp
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
  //Fin de sección común

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





Salida de la ejecución:

~~~bash
> mpirun -np 2 sendrecv1
Proceso 0 ha enviado 100 al proceso 1
Proceso 1 ha recibido 100 de proceso 0
~~~

El mismo programa es ejecutado por los dos procesos (-np 2). El proceso al que MPI indentifique como 0 (rank==0) entrará en el primer if y realizará el envío del mesaje mientras que el otro proceso que habrá sido indentificado como rank=2 saltará la condición y ejecutará el código dentro del else. Si ejecutáramos el mismo programa para tres procesos de la misma forma el proceso 0 enviaría un mensaje al proceso 1, este lo recibiría e imprimiría por pantalla, pero el proceso 2 quedaría esperando la recepción y la ejecución quedaría colgada indefinidamente.

~~~bash
> mpirun -np 3 sendrecv1
Proceso 0 ha enviado 100 al proceso 1
Proceso 1 ha recibido 100 de proceso 0

^Cmpirun: killing job...

--------------------------------------------------------------------------
mpirun noticed that process rank 0 with PID 5997 on node juan-Aspire-5920 exited on signal 2 (Interrupt).
--------------------------------------------------------------------------
3 total processes killed (some possibly by mpirun during cleanup)
~~~

Esto podría solucionarse si le decimos al proceso 0 que también envíe un mensaje al proceso 2.

~~~c
> mpicxx -o sendrecv1 sendrecv1.cpp
> mpirun -np 3 sendrecv1
Proceso 0 ha enviado 100 al proceso 1
Proces0 0 ha enviado 100 al proceso 2
Proceso 1 ha recibido 100 de proceso 0
Proceso 2 ha recibido 100 de proceso 0
~~~

###Emparejamiento de operaciones de envío y recepción:

En MPI una operación de envío de un mensjae (con tag e) desde un proceso A a un proceso B (con una operación de recepcción en B) sólo podrá realizarse si se cumplen una de estas tres condiciones:

* A nombra a B como receptor y e como etiqueta. Es decir que A diga que va a enviar a B en su campo destino de Send y que B diga que recive  desde A en su campo source y que además ambos tengan la misma etiqueta y estén dentro del mismo comunicador.
* B diga MPI_ANY_SOURCE, entonces B puede recibir de cualquier origen (source). Siempre que tengan el mismo tag, de no ser así no se recibirá el mensaje.
      MPI_Recv ( &value, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status );
El mensaje sigue recibiéndose porque se usa la misma tag y se recibiría de cualquier origen.

* B diga MPY_ANY_TAG, entonces recibirá desde cualquier tag.

Si una operación encajara con varias posibilidades de envío o recepcción entonces se seleccionará de entre esas varias la primera en iniciarse (Esto facilita garantizar propiedades de equidad).



###Compilación y ejecución de MPI

Para compilar un programa con OpenMPI necesitamos usar **script específicos** :

  * Para compilar: **mpicxx**
  * Para ejecutar: **mpirun**

Ejemplo:

    mpicxx -o holamundo holamundo.cpp

    mpirun -np 4 holamundo

    //Donde con np especificamos el número de procesos que queremos que ejecuten el código.
