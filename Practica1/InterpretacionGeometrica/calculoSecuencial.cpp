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

		cout << "Nº de Muestras: " << m << endl;
		cout << "Resultado de la suma: " << suma/m << endl;
		cout << "Tiempo transcurrido: " << duracion(&inicio, &fin) << " seg. " << endl;
}