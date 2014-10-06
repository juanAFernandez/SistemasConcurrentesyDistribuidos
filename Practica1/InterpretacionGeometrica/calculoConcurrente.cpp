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

//Implementación de la función f(x)=4/(1+x^2):
double funcion(double x){
	return 4.0/(1+x*x);

}

struct datos{
	double primerPuntoMedio;
	double base;
	double area;
	int m;
};


//Función que ejecuta cada hebra:
void * funcion_hebra(void * arg){

	//Obteniendo los datos de la estructura pasada como argumento.
	struct datos *p;
	p = (struct datos *) arg;

	double sumaParcial=0.0;
	double puntoMedio=p->primerPuntoMedio;
	int m=p->m;
	double area=p->area;
	double base=p->base;

	for(int i=0; i<m; i++){
		area += base*funcion(puntoMedio); // es igual a area= area+(base*funcion..)
		puntoMedio += base;
	}

	p->area=area;


}

//Función que lanza la ejecución de las hebras:
double calcular_integral_concurrente(int m){
	
	//Variable area necesaria para el cálculo.
	double area=0.0;

	//La base de todos los rectángulos.
	double base = 1/(double)m;

	//Declaración de los struct:
	struct datos seccionA;
	struct datos seccionB;

	//Asignación de los puntos medios de los primeros rectangulos a calcular el area.
	seccionA.primerPuntoMedio=base/2;
	seccionB.primerPuntoMedio=(base*(m/2))+seccionA.primerPuntoMedio;

	seccionA.area=area;
	seccionB.area=area;

	seccionA.base=base;
	seccionB.base=base;

	seccionA.m=m/2;
	seccionB.m=m/2;

	//Muestra contenido de los struct:
	/*
	cout << "Seccion A: " << endl;
	cout << "Primer punto medio: " << seccionA.primerPuntoMedio << endl;
	cout << "Base: " << seccionA.base << endl;
	cout << "Area: " << seccionA.area << endl;
	cout << "Muestreo: " << seccionA.m << endl;

	//Muestra contenido de los struct:

	cout << "Seccion B: " << endl;
	cout << "Primer punto medio: " << seccionB.primerPuntoMedio << endl;
	cout << "Base: " << seccionB.base << endl;
	cout << "Area: " << seccionB.area << endl;
	cout << "Muestreo: " << seccionB.m << endl;
	*/


	//Declaración de las hebras:
	pthread_t hebra1, hebra2;

	//Creación de la hebras asociándolas a una función concreta que ejecutaran:
	pthread_create(&hebra1, NULL, funcion_hebra, (void *)&seccionA);
	pthread_create(&hebra2, NULL, funcion_hebra, (void *)&seccionB);

	//Punto de espera para cada una de las hebras:
	pthread_join(hebra1,NULL);
	pthread_join(hebra2,NULL);

	return seccionA.area+seccionB.area;

}



int main(int argc, char *argv[]){

	int m = atoi(argv[1]);

	if(m%2!=0){
		cout << "El número de muestras debe ser par." << endl;
		return 0;
	}

	double resultado;
	
	//Capturamos el tiempo exacto en el momento de la ejecución de la orden.
	struct timespec inicio=ahora();
		
		resultado = calcular_integral_concurrente(m);

	//Volvemos a capturar el tiempo en el instante de la ejecución de la orden.
	struct timespec fin=ahora();

		cout << "### Versión Concurrente ###" << endl;
		cout << "Nº de Muestras: " << m << endl;
		cout << "Resultado de la suma: " << resultado << endl;
		cout << "Tiempo transcurrido: " << duracion(&inicio, &fin) << " seg. " << endl;
}