/*

Versión 1.0

Cálculo secuencial de la integral de la función 4/(1*x^2) entre 0 y 1 que da como resultado el número PI=3.1415... 
como area de esa porción de función. Al programa se le pasa el número de muestras en el que queremos que se divida la función 
para el cálculo de su area.
*/
#include <iostream>
#include "fun_tiempo.h"
#include "fun_tiempo.c"
using namespace std;

double funcion(double x){
	return 4.0/(1+x*x);
}

int main(int argc, char *argv[]){

	int m = atoi(argv[1]);
	
	double suma;
	
	struct timespec inicio=ahora();
	
	for(int i=0; i<m; i++)
		suma += funcion((i+0.5)/m);
	
	struct timespec fin=ahora();
		
	cout << "### Versión Secuencial ###" << endl;
	cout << "Nº de Muestras: " << m << endl;
	cout << "Resultado de la suma: " << suma/m << endl;
	cout << "Tiempo transcurrido: " << duracion(&inicio, &fin) << " seg. " << endl;
}