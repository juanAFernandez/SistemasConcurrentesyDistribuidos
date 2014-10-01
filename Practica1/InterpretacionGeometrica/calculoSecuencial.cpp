#include <iostream>
using namespace std;

unsigned long m = 100000000;

double funcion(double x){
	return 4.0/(1+x*x);
}

int main(){

	double suma;

	for(int i=0; i<m; i++)
		suma += funcion((i+0.5)/m);

	cout <<  suma/m;
}