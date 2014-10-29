/*
## Problema Productor-Consumidor ##

Problema básico para creación de hebras y sincronización de las mismas usando semáforos.

Funcionamiento:



@Autor: Juan Antonio Fernández Sánchez
@Fecha: Octubre 2014

Para compilar y ejecutar:
Ubuntu> g++ productorConsumidor.cpp -o productorConsumidor -lpthread ; ./productorConsumidor 
*/

#include <iostream>
//Para el uso de hebras POSIX
#include <pthread.h> 
//Para el uso de semáforos POSIX para la sincronización de las hebras
#include <semaphore.h>

#include <stdlib.h>


using namespace std;


//Uso de variable globales para que puedan ser accedidas desde cualquier lugar.

sem_t puedeProducir;
sem_t puedeConsumir;
sem_t mutex;

//Definición de tiempos aleatorios de ejecución de hebras
const int tiempoHebraA=2;//rand()%5+1;
const int tiempoHebraB=4;//rand()%5+1;


int contenedorIntermedio=0;

//Buffer de 5 elementos
const int tamBuffer=4;
int buffer[tamBuffer];

//Variable compartida por las dos hebras
int primeraLibre=0;


void imprimirBuffer(int buffer[], int tam){
	cout << "Buffer:	";
	for(int i=0; i<tam; i++)
		cout << "[" << buffer[i] << "]";
	cout << endl;
}



//Función para hebra productora
void* producir(void *p){

	int valor=1;

	while(true){

		sem_wait(&puedeProducir);

		//Acción de producir:
		sleep(tiempoHebraA);
		buffer[primeraLibre]=rand()%10+1;
		cout << "Insertado en el buffer: " << buffer[primeraLibre] << endl;
		//FIN

		primeraLibre++;
		sem_post(&mutex);

		//imprimirBuffer(buffer, tamBuffer);
		//La teórica sem_signal
		sem_post(&puedeConsumir);
	}

	return NULL;

}

//Función para la hebra consumidora
void* consumir(void *p){

	while(true){

		sem_wait(&puedeConsumir);

		//Acción de consumir:
		sleep(tiempoHebraB);
		//cout << "pos primera libre: " << primeraLibre << endl;
		cout << "Leido del buffer: " << buffer[primeraLibre-1] << endl;

		sem_wait(&mutex);
		primeraLibre--;

		//FIN

		sem_post(&puedeProducir);

	}
	return NULL;
}




int main(){

	//Mensaje de entrada
	cout << endl << " ########### Productor-Consumidor ########### " << endl;
	cout << " tiempos para las hebras:" << endl;
	cout << "	hebraA: " << tiempoHebraA << " seg." << endl;
	cout << "	hebraB: " << tiempoHebraB << " seg." << endl << endl;


	/*Inicialización del semáforo puedeEscribir a 4. Esta inicialización es así debido a que si el buffer tiene cuatro
	celdas vacías el sermáforo de puedeProducir debe empezar a cuatro.
	*/
	sem_init(&puedeProducir, 0, 4);
	//Inicialización del semáforo puedeLeer a 0
	sem_init(&puedeConsumir, 0, 0);

	//Necesitamos otro semáforo a 1 para que pueda escribir en primeraLibre.
	sem_init(&mutex, 0, 0);


	//Como el procesador del equipo sólo tiene dos nucleos usamos dos hebras.
	pthread_t productora, consumidora;

	pthread_create(&productora, NULL, producir, NULL);
	pthread_create(&consumidora, NULL, consumir, NULL);


	//Punto de espera a las hebras
	pthread_join(productora,NULL);
	pthread_join(consumidora,NULL);

	//Liberación de la memoria ocupada por las estructuras de los semáforos.
	sem_destroy(&puedeProducir);
	sem_destroy(&puedeConsumir);
	sem_destroy(&mutex);

}