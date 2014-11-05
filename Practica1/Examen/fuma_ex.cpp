/*
## Problema fumadores de EXAMEN ##

@brief En este caso habrá 6 hebras fumadoras. Cada vez que el estanquero coloque un ingrediente en la mesa despierta
a uno de los dos fumadores que necesitan ese ingrediente (da igual cual). El estanquero podrá colocar dos ingredientes
en el mostrador (de forma aleatoria), entonces sólo se debe bloquear cuando haya dos ingredientes sin retirar en la mesa.

@Autor: Juan Antonio Fernández Sánchez
@Fecha: Noviembre 2014
@version: examen

*/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
using namespace std;



//El mostrador será considerado como dos unidades donde puede haber tres cosas:
//	0--> ningun ingrediente		1--> tabaco 		2--> papel		3--> cerillas
int mostradorA=0;
int mostradorB=0;

//Semáforo para la exclusión mutua de las dos unidades del mostrador.
sem_t mutexA;

sem_t 
	semaforoFumadorA, 
	semaforoFumadorB, 
	semaforoFumadorC,
	semaforoFumadorD,
	semaforoFumadorE,
	semaforoFumadorF;

sem_t
	semEstanquero;

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
		mostradorA = rand()%3+1;
		mostradorB = rand()%3+1;	
		
		cout << "En el mostradorA se deja: ";
		if(mostradorA == 1)
			cout << "tabaco" << endl;
		if(mostradorA == 2)
			cout << "papel" << endl;
		if(mostradorA == 3)
			cout << "cerillas" << endl;

		cout << "En el mostradorB se deja: ";
		if(mostradorB == 1)
			cout << "tabaco" << endl;
		if(mostradorB == 2)
			cout << "papel" << endl;
		if(mostradorB == 3)
			cout << "cerillas" << endl;


		// Según el ingrediente que se produzca se desbloquea a un fumador o a otro
		
		//	0--> ningun ingrediente		1--> tabaco 		2--> papel		3--> cerillas

		int eleccionFumador;

		//Si en el mostrador A o en el mostrador B se deja tabaco...
		if( (mostradorA==1) || (mostradorB==1) ){
		   // ... se selecciona al azar uno de los dos fumadores que lo necesitan (el B o el E)...
		   eleccionFumador=rand()%2+1;
		   if(eleccionFumador==1)
		   	// ... y se desbloquea.
		   	sem_post(&semaforoFumadorB);
		   else if(eleccionFumador==2)
		   	// ... y se desbloquea.
		   	sem_post(&semaforoFumadorE);
		}


		//Si en el mostrador A o en el mostrador B se dejan cerillas...
		if( (mostradorA==3) || (mostradorB==3) ){
		   // ... se selecciona al azar uno de los dos fumadores que lo necesitan (el A o el D)...
		   eleccionFumador=rand()%2+1;
		   if(eleccionFumador==1)
		   	// ... y se desbloquea.
		   	sem_post(&semaforoFumadorA);
		   else if(eleccionFumador==2)
		   	// ... y se desbloquea.
		   	sem_post(&semaforoFumadorD);
		}

		//Si en el mostrador A o en el mostrador B se deja papel...
		if( (mostradorA==2) || (mostradorB==2) ){
		   // ... se selecciona al azar uno de los dos fumadores que lo necesitan (el C o el F)...
		   eleccionFumador=rand()%2+1;
		   if(eleccionFumador==1)
		   	// ... y se desbloquea.
		   	sem_post(&semaforoFumadorC);
		   else if(eleccionFumador==2)
		   	// ... y se desbloquea.
		   	sem_post(&semaforoFumadorF);
		}
		

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
	//Si su ingrediente está en el mostrador A
	if(mostradorA==3)
		//Consume el ingrediente
		mostradorA=0;
	//Si su ingrediente está en el mostrador B
	if(mostradorB==3)
		//Consume el ingrediente
		mostradorB=0;

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
	//Si su ingrediente está en el mostrador A
	if(mostradorA==1)
		//Consume el ingrediente
		mostradorA=0;
	//Si su ingrediente está en el mostrador B
	if(mostradorB==1)
		//Consume el ingrediente
		mostradorB=0;
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
	//Si su ingrediente está en el mostrador A
	if(mostradorA==2)
		//Consume el ingrediente
		mostradorA=0;
	//Si su ingrediente está en el mostrador B
	if(mostradorB==2)
		//Consume el ingrediente
		mostradorB=0;
	cout << "Fumador C recoge papel y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador C ha terminado de fumar! " << endl;
	

	}//Fin while

}

void* fumaFumadorD(void *p){

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorD);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	//Si su ingrediente está en el mostrador A
	if(mostradorA==3)
		//Consume el ingrediente
		mostradorA=0;
	//Si su ingrediente está en el mostrador B
	if(mostradorB==3)
		//Consume el ingrediente
		mostradorB=0;
	cout << "Fumador D recoge cerillas y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador D ha terminado de fumar! " << endl;
	

	}//Fin while

}

void* fumaFumadorE(void *p){

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorE);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	//Si su ingrediente está en el mostrador A
	if(mostradorA==1)
		//Consume el ingrediente
		mostradorA=0;
	//Si su ingrediente está en el mostrador B
	if(mostradorB==1)
		//Consume el ingrediente
		mostradorB=0;
	cout << "Fumador E recoge tabaco y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador E ha terminado de fumar! " << endl;
	

	}//Fin while

}

void* fumaFumadorF(void *p){

	while(true){

	//El fumador A no puede fumar hasta que el estanquero lo desbloquee cuando haya servido su ingrediente
	sem_wait(&semaforoFumadorF);
	//Tras entrar una vez no podrá volver a entrar hasta que alguien vuelva a desbloquear su entrada.


	//Recoger el producto
	sem_wait(&mutex); //Se encierra en una zona de exclusión mutua para asegurar que mientras que el accede al mostrador del estanquero nadie lo haga.
	//Si su ingrediente está en el mostrador A
	if(mostradorA==2)
		//Consume el ingrediente
		mostradorA=0;
	//Si su ingrediente está en el mostrador B
	if(mostradorB==2)
		//Consume el ingrediente
		mostradorB=0;
	cout << "Fumador F recoge papel y se pone a fumar! " << endl;
	//Se recoge el producto y se libera al estanquero para que produzca otro.
	sem_post(&semEstanquero);
	sem_post(&mutex); //Se libera el mostrador para que lo pueda utilizar quien quiera.

	fumar();
	cout << "Fumador F ha terminado de fumar! " << endl;
	

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
	sem_init(&semaforoFumadorD, 0, 0);
	sem_init(&semaforoFumadorE, 0, 0);
	sem_init(&semaforoFumadorF, 0, 0);

	sem_init(&semEstanquero,0,1);
	
	pthread_t estanquero, fumadorA, fumadorB, fumadorC, fumadorD, fumadorE, fumadorF;

	pthread_create(&estanquero, NULL, suministrar, NULL);

	pthread_create(&fumadorA, NULL, fumaFumadorA, NULL);
	pthread_create(&fumadorB, NULL, fumaFumadorB, NULL);
	pthread_create(&fumadorC, NULL, fumaFumadorC, NULL);
	pthread_create(&fumadorD, NULL, fumaFumadorD, NULL);
	pthread_create(&fumadorE, NULL, fumaFumadorE, NULL);
	pthread_create(&fumadorF, NULL, fumaFumadorF, NULL);


	//Punto de espera a las hebras
	pthread_join(fumadorA, NULL);
	pthread_join(fumadorB, NULL);
	pthread_join(fumadorC, NULL);
	pthread_join(fumadorD, NULL);
	pthread_join(fumadorE, NULL);
	pthread_join(fumadorF, NULL);
	pthread_join(estanquero,NULL);

	//Destruimos todos los semáforos creados.
	
	sem_destroy(&mutex);

	sem_destroy(&semaforoFumadorA);
	sem_destroy(&semaforoFumadorB);
	sem_destroy(&semaforoFumadorC);
	sem_destroy(&semaforoFumadorD);
	sem_destroy(&semaforoFumadorE);
	sem_destroy(&semaforoFumadorF);

	sem_destroy(&semEstanquero);

	
}