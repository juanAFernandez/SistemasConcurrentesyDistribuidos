#include <iostream>
#include "fun_tiempo.h"
#include "fun_tiempo.c"
using namespace std;

unsigned long m = 100000;

double funcion(double x){
	return 4.0/(1+x*x);
}

int main(){

	double suma;
	
	struct timespec inicio=ahora();
	
	for(int i=0; i<m; i++)
		suma += funcion((i+0.5)/m);
	
	struct timespec fin=ahora();

		cout << suma/m;
		cout << "Tiempo transcurrido == " << duracion(&inicio, &fin) << " seg. " << endl;
}