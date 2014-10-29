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

//sem_t puede_escribir;
//sem_t puede_leer;
sem_t s1;

//unsigned long valor_compartido;

int contenedorIntermedio=0;

//const unsigned long num_iter=10000;


//Función para hebra productora
void* escribir(void *p){

	sleep(3);
	contenedorIntermedio=1;
	//La teórica sem_signal
	sem_post(&s1);

	/*
	unsigned long contador=0;
	for(unsigned long i=0; i<num_iter; i++){
		contador=contador+1;
		sem_wait(&puede_escribir);
		valor_compartido=contador;
		sem_post(&puede_leer);

		sem_wait(&mutex);
		cout << "valor escrito == " << contador << endl << flush;
		sem_post(&mutex);
	}
	*/
	return NULL;

}

//Función para la hebra consumidora
void* leer(void *p){



	sem_wait(&s1);
	cout << "Valor: " << contenedorIntermedio << endl;

	/*
	unsigned long valor_leido;
	for(unsigned long i=0; i<num_iter; i++){
		sem_wait(&puede_leer);
		valor_leido=valor_compartido;
		sem_post(&puede_escribir);

		sem_wait(&mutex);
		cout << "valor leido == " << valor_leido << endl << flush;
		sem_wait(&mutex);
	}*/
	return NULL;
}




int main(int argc, char *argv[]){

	//Inicialización de los semáforos
	//sem_init(&puede_escribir, 0, 1);
	//sem_init(&puede_leer, 0, 0);
	//El tercer valor es el valor inicial del semáforo
	sem_init(&s1, 0, 0);


	//Como el procesador del equipo sólo tiene dos nucleos usamos dos hebras.
	pthread_t hebra1, hebra2;

	pthread_create(&hebra1, NULL, escribir, NULL);
	pthread_create(&hebra2, NULL, leer, NULL);


	//Punto de espera a las hebras
	pthread_join(hebra1,NULL);
	pthread_join(hebra2,NULL);

	//Destrucción de los semáforos
	//sem_destroy(&puede_escribir);
	//sem_destroy(&puede_leer);
	sem_destroy(&s1);

}