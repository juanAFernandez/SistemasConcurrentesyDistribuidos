/*
## Problema FUMADORES ##

Problema más complejo para creación de hebras y sincronización de las mismas usando semáforos.

Funcionamiento:


@Autor: Juan Antonio Fernández Sánchez
@Fecha: Noviembre 2014

Para compilar y ejecutar:
g++ -o fumadores fumadores.cpp -lpthread; ./fumadores
*/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
using namespace std;

struct fumador{
	string nombre;
	bool tabaco;
	bool papel;
	bool cerillas;
};

//El mostrador será considerado como una única unidad donde puede haber tres cosas:
//	0--> ningun ingrediente		1--> tabaco 		2--> papel		3--> cerillas
int mostrador=0;

sem_t mutex, semaforoFumadorA, semaforoFumadorB, semaforoFumadorC, semEstanquero;

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

void* fumaFumadorA(void *p){

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorA);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	mostrador=0;
	cout << "Fumador A recoge cerillas y se pone a fumar! " << endl;
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

	// Inicializamos la semilla aleatoria
	srand(time (NULL));




	sem_init(&mutex, 0, 1);
	//El semáforo que bloquea al fumador A debe inicializarse a 0 ya que al empezar la ejecución este no tendrá por que tener su ingrediente en el mostrador.
	sem_init(&semaforoFumadorA, 0, 0);
	sem_init(&semaforoFumadorB, 0, 0);
	sem_init(&semaforoFumadorC, 0, 0);

	sem_init(&semEstanquero,0,1);
	
	pthread_t estanquero, fumadorA, fumadorB, fumadorC;


	pthread_create(&estanquero, NULL, suministrar, NULL);


	pthread_create(&fumadorA, NULL, fumaFumadorA, NULL);
	pthread_create(&fumadorB, NULL, fumaFumadorB, NULL);
	pthread_create(&fumadorC, NULL, fumaFumadorC, NULL);

	//pthread_create(&fumadorB, NULL, fumar, NULL);
	//pthread_create(&fumadorC, NULL, fumar, NULL);


	//Punto de espera a las hebras
	//pthread_join(productora,NULL);
	pthread_join(fumadorA, NULL);
	pthread_join(fumadorB, NULL);
	pthread_join(fumadorC, NULL);
	pthread_join(estanquero,NULL);

	sem_destroy(&mutex);
	sem_destroy(&semaforoFumadorA);
	sem_destroy(&semaforoFumadorB);
	sem_destroy(&semaforoFumadorC);
	sem_destroy(&semEstanquero);

	
}