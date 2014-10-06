/*
## VERSION 1 ##
División central de la función para las hebras para un número fijo de procesadores = 2

Cálculo concurrente de la integración.
En este programa vamos a dividir la función en segmentos y para cada segmento le vamos
a calcular el área. La suma de todas las areas será la aproximación del area que 
se dibuja debajo de la función entre 0 y 1. Valor que conocemos de antemano, PI=3,1416...
*/
#include <iostream>
#include <pthread.h>
#include "fun_tiempo.h"
#include "fun_tiempo.c"
using namespace std;

//Para calcular el valor de la función para el valor x dado.
//Es decir la altura de los rectángulos para calcular el area de los mismos.

//Implementación de la función f(x)=4/(1+x^2)
double funcion(double x){
	return 4.0/(1+x*x);
}


//Función que ejecuta cada hebra:
void * funcion_hebra(void * ih_void){
	double sumaParcial=0.0;
}

//Función que lanza la ejecución de las hebras:
double calcular_integral_concurrente(){
	//Declaración de las hebras:
	pthread_t hebra1, hebra2;
	pthread_create(&hebra1, NULL, funcion_hebra, NULL);
	pthread_create(&hebra2, NULL, funcion_hebra, NULL);

}



int main(int argc, char *argv[]){

	int m = atoi(argv[1]);

	if(m%2!=0){
		cout << "El número de muestras debe ser par." << endl;
		return 0;
	}

	
	double area=0.0;

	//La base de todos los rectángulos.
	double base = 1/(double)m;
	double primerPuntoMedio=base/2;

	//cout << "base:" << base << endl;

	double puntoMedio=primerPuntoMedio;
	
	//Capturamos el tiempo exacto en el momento de la ejecución de la orden.
	struct timespec inicio=ahora();
		
	for(int i=0; i<m; i++){
		area += base*funcion(puntoMedio); // es igual a area= area+(base*funcion..)
		puntoMedio += base;

		cout << puntoMedio << endl;
		cout << funcion(puntoMedio) << endl;
	}
	
	//Volvemos a capturar el tiempo en el instante de la ejecución de la orden.
	struct timespec fin=ahora();

		cout << "Nº de Muestras: " << m << endl;
		cout << "Resultado de la suma: " << area << endl;
		cout << "Tiempo transcurrido: " << duracion(&inicio, &fin) << " seg. " << endl;
}