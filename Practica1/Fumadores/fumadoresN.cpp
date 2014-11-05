/*
## Problema FUMADORES ##

En este caso el problema se translada a n fumadores. Por lo que tendremos que generalizar la función de fumar 
y crear una estructura que represente a un fumador y al ingrediente que le falta, mientras que el estanquero tendrá que conocer
el número de fumadores que hay (tendremos que almacenarlos en alguna estructura como un vector) y recorrerlo para saber a cual
desbloquear.
Antes usábamos una hebra fumadora por cada fumador ahora debemos usar una función genérica para cualquier número n de
fumadores.

@Autor: Juan Antonio Fernández Sánchez
@Fecha: Noviembre 2014

Para compilar y ejecutar:
g++ -o fumadoresN fumadoresN.cpp -lpthread; ./fumadoresN
*/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
using namespace std;

//El mostrador será considerado como una única unidad donde puede haber tres cosas:
//	0--> ningun ingrediente		1--> tabaco 		2--> papel		3--> cerillas
int mostrador=0;

// ## Declaración de semáforos ## //
//Necesitamos un semáforo para asegurar la exclusión mutua, que será usado por todos los fumadores y por el estanquero.
sem_t mutex;

//Necesitamos otro semáforo con el que bloquear al estanquero.
sem_t semEstanquero;

//Necesitamos otros tantos sermáforos como fumadores tengamos para poder bloquearlos.
sem_t semaforoFumadorA, semaforoFumadorB, semaforoFumadorC;



struct fumador{
	string nombre;
	int ingredienteQueFalta;
};


void* suministrar(void *p){

	/*
		"El estanquero selecciona aleatoriamente un ingrediente de los tres que se necesitan, lo pone en el mostrador,
		desbloquea al fumador que necesita dicho ingrediente y después se bloquea, esperando la retirada del ingrediente.
	*/

	while(true){

 	//Miestras el estanquero no esté libre no podrá producir
	sem_wait(&semEstanquero);

	sem_wait(&mutex);

		//Acción de poner ingredientes en el mostrador
		// Selección aleatoria del ingrediente a suministrar
		mostrador = rand()%3+1;	
		cout << "En el mostrador se deja: ";
		if(mostrador == 1)
			cout << "tabaco" << endl;
		if(mostrador == 2)
			cout << "papel" << endl;
		if(mostrador == 3)
			cout << "cerillas" << endl;

		// Según el ingrediente que se produzca se desbloquea a un fumador o a otro
		
		if(mostrador == 1)
		   sem_post(&semaforoFumadorB);

		if(mostrador == 2)
			sem_post(&semaforoFumadorC);

		//Al fumador A le faltan cerillas: cuando se sirva el ingrediente nº tres se desbloquea a este fumador.
		if(mostrador == 3)
		   sem_post(&semaforoFumadorA);

		

	sem_post(&mutex);



	}//FIN WHILE

}

void fumar(){
	//Calcula un número aleatorio de milisegundos (entre 1/10 y 2 segundos)
	const unsigned miliseg = 100U + (rand() % 1900U);
	usleep(1000U*miliseg); //retraso bloqueado durante miliseg milisegundos.
}

//Esta función recibe como parámetro una estructura (el propio fumador que realiza la acción).
void* fumaFumadorA(void *arg){

	struct fumador *fumadorTemporal;
	fumadorTemporal=(struct fumador *) arg;

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorA);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	//El ingrediente se saca del mostrador, es decir, el mostrador se deja a 0.
	mostrador=0; //La exclusión mutua principalmente para esto, pero aprovechamos y mostramos mensajes para que no se entremezclen y que se pueda depurar mejor.
	cout << "Fumador " << fumadorTemporal->nombre << " recoge ";
	if(fumadorTemporal->ingredienteQueFalta==1)
		cout << "tabaco";
	if(fumadorTemporal->ingredienteQueFalta==2)
		cout << "papel";
	if(fumadorTemporal->ingredienteQueFalta==3)
		cout << "cerillas";
	cout << " y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador A ha terminado de fumar! " << endl;
	

	}//Fin while

}

void* fumaFumadorB(void *p){

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorB);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	mostrador=0;
	cout << "Fumador B recoge tabaco y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador B ha terminado de fumar! " << endl;
	

	}//Fin while

}

void* fumaFumadorC(void *p){

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorC);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	mostrador=0;
	cout << "Fumador C recoge papel y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador C ha terminado de fumar! " << endl;
	

	}//Fin while

}



int main(){

	//	0--> ningun ingrediente		1--> tabaco 		2--> papel		3--> cerillas

	//Declaración de los objetos tipo fumador:
	fumador fumadorA;
	fumadorA.nombre="A";
	fumadorA.ingredienteQueFalta=3;


	// Inicializamos la semilla aleatoria
	srand(time (NULL));

	sem_init(&mutex, 0, 1);
	//El semáforo que bloquea al fumador A debe inicializarse a 0 ya que al empezar la ejecución este no tendrá por que tener su ingrediente en el mostrador.
	sem_init(&semaforoFumadorA, 0, 0);
	sem_init(&semaforoFumadorB, 0, 0);
	sem_init(&semaforoFumadorC, 0, 0);

	sem_init(&semEstanquero,0,1);
	
	pthread_t estanquero, hebraFumadorA, hebraFumadorB, hebraFumadorC;


	pthread_create(&estanquero, NULL, suministrar, NULL);

	//Se cran las hebras y se lanzan.
	pthread_create(&hebraFumadorA, NULL, fumaFumadorA, (void *)&fumadorA); //A esta le pasamos en el param 4 la estructura fumadorA
	pthread_create(&hebraFumadorB, NULL, fumaFumadorB, NULL);
	pthread_create(&hebraFumadorB, NULL, fumaFumadorC, NULL);


	//Punto de espera a las hebras
	pthread_join(hebraFumadorA, NULL);
	pthread_join(hebraFumadorB, NULL);
	pthread_join(hebraFumadorC, NULL);
	pthread_join(estanquero,NULL);

	//Destrucción de los semáforos
	sem_destroy(&mutex);
	sem_destroy(&semaforoFumadorA);
	sem_destroy(&semaforoFumadorB);
	sem_destroy(&semaforoFumadorC);
	sem_destroy(&semEstanquero);

	
}