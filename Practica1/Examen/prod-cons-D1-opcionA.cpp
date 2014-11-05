#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;


const unsigned 
	num_items = 30, 
	tam_vec = 5 ;
int 
	vector[tam_vec],
        n_eltos = 0;

unsigned 
	primera_libre = 0,
	primera_ocupada = 0;

pthread_t 
	hebra_productor, 
	hebra_consumidor ;
sem_t 
	puede_escribir, 
	puede_leer,
	mutex ;

//----------------------------------------------------------------------

int producir_dato()
{
    static int contador = 1 ;
    return contador ++ ;
}

//----------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}

//----------------------------------------------------------------------

void * productor( void * )
{
    for( unsigned i = 0 ; i < num_items ; i++ )
    {   
	int dato = producir_dato() ;
		
	sem_wait( &puede_escribir ) ;
 
	vector[primera_libre] = dato ;
	primera_libre = primera_libre + 1 ;	

       sem_wait( &mutex ) ;
               n_eltos++;
         sem_post( &mutex ) ;

        sem_post( &puede_leer ) ;
    }
}

//----------------------------------------------------------------------

void * consumidor( void * )
{
    for( unsigned i = 0 ; i < num_items ; i++ )
    {   
	int dato ;
		
        sem_wait( &puede_leer );

	dato = vector[primera_ocupada] ;
        primera_ocupada = primera_ocupada + 1 ;    
           

        sem_wait( &mutex ) ;
               n_eltos--;
 	sem_post( &mutex ) ;

	sem_post (&puede_escribir);
      
        consumir_dato( dato ) ;
    }
}

//----------------------------------------------------------------------

int main()
{
   sem_init( &puede_escribir, 0, tam_vec); 
   sem_init( &puede_leer, 0, 0); 
   sem_init( &mutex, 0, 1 ) ;  

   pthread_create(&hebra_productor,  NULL, productor,  NULL );
   pthread_create(&hebra_consumidor, NULL, consumidor, NULL );
   
   pthread_join( hebra_productor,  NULL ) ;
   pthread_join( hebra_consumidor, NULL ) ;

   sem_destroy( &puede_escribir );
   sem_destroy( &puede_leer ); 
}


