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

sem_t s1;



int contenedorIntermedio=0;



//Función para hebra productora
void* escribir(void *p){

	//Acción de producir:
	sleep(2);
	contenedorIntermedio=1;

	//La teórica sem_signal
	sem_post(&s1);

	return NULL;

}

//Función para la hebra consumidora
void* leer(void *p){



	sem_wait(&s1);
	//Acción de consumir:
	sleep(1);
	cout << "Valor: " << contenedorIntermedio << endl;

	return NULL;
}




int main(){

	sem_init(&s1, 0, 0);


	//Como el procesador del equipo sólo tiene dos nucleos usamos dos hebras.
	pthread_t hebra1, hebra2;

	pthread_create(&hebra1, NULL, escribir, NULL);
	pthread_create(&hebra2, NULL, leer, NULL);


	//Punto de espera a las hebras
	pthread_join(hebra1,NULL);
	pthread_join(hebra2,NULL);


	sem_destroy(&s1);

}