/*

Versión 1.1

Cálculo secuencial de la integral de la función 4/(1*x^2) entre 0 y 1 que da como resultado 
el número PI=3.1415... como area de esa porción de función.
Al programa se le pasa el número de muestras en el que queremos que se divida la función 
para el cálculo de su area.

*/
#include <iostream>
#include "fun_tiempo.h"
#include "fun_tiempo.c"
using namespace std;

//Para calcular el valor de la función para el valor x dado.
//Es decir la altura de los rectángulos para calcular el area de los mismos.
double funcion(double x){
	return 4.0/(1+x*x);
}

int main(int argc, char *argv[]){

	int m = atoi(argv[1]);
	
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

		//cout << puntoMedio << endl;
		//cout << funcion(puntoMedio) << endl;
	}
	
	//Volvemos a capturar el tiempo en el instante de la ejecución de la orden.
	struct timespec fin=ahora();

	double tiempo=duracion(&inicio, &fin);
	//Salida para su uso posterior con gnuplot		
	cout  << m << "  " << tiempo;
}