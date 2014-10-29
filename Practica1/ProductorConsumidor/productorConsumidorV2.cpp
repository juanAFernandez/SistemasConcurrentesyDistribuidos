/*
Productor - Consumidor
Creación de hebras y sincronización de las mismas usando semáforos

Para compilar y ejecutar:
Ubuntu> g++ productorConsumidor.cpp -o productorConsumidor -lpthread ; ./productorConsumidor 

*/

#include <iostream>
//Para el uso de hebras POSIX
#include <pthread.h> 
//Para el uso de semáforos POSIX para la sincronización de las hebras
#include <semaphore.h> 

using namespace std;


//Uso de variable globales para que puedan ser accedidas desde cualquier lugar.

sem_t puedeLeer;
sem_t puedeEscribir;



int contenedorIntermedio=0;



//Función para hebra productora
void* escribir(void *p){

	int valor=1;

	while(true){

		sem_wait(&puedeEscribir);

		//Acción de producir:
		sleep(1);
		contenedorIntermedio=valor;
		cout << "Escrito en el contenedor: " << contenedorIntermedio << endl;
		valor++;
		//FIN

		//La teórica sem_signal
		sem_post(&puedeLeer);
	}

	return NULL;

}

//Función para la hebra consumidora
void* leer(void *p){

	while(true){

		sem_wait(&puedeLeer);

		//Acción de consumir:
		sleep(1.5);
		cout << "Leido del contenedor: " << contenedorIntermedio << endl;
		//FIN

		sem_post(&puedeEscribir);

	}
	return NULL;
}




int main(){

	//Inicialización del semáforo puedeEscribir a 1
	sem_init(&puedeEscribir, 0, 1);
	//Inicialización del semáforo puedeLeer a 0
	sem_init(&puedeLeer, 0, 0);


	//Como el procesador del equipo sólo tiene dos nucleos usamos dos hebras.
	pthread_t hebra1, hebra2;

	pthread_create(&hebra1, NULL, escribir, NULL);
	pthread_create(&hebra2, NULL, leer, NULL);


	//Punto de espera a las hebras
	pthread_join(hebra1,NULL);
	pthread_join(hebra2,NULL);

	//Liberación de la memoria ocupada por las estructuras de los semáforos.
	sem_destroy(&puedeEscribir);
	sem_destroy(&puedeLeer);

}