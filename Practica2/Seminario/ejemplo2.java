/*
  Ahora vamos a usar vectores de hebras para complicar un poco más el código.
*/

import java.util.Random;

class TipoHebra implements Runnable{


  //Número de veces que la hebra duerme
  int vueltas;
  //Tiempo que duerme la hebra
  int siesta;
  //Objeto de tipo hebra encapsulado, lo necesitamos para poder usar las hebras.
  public Thread thr;


  //Constructor de la clase, que inicializa los atributos de la clase.
  public TipoHebra(String nombre, int siesta, int vueltas){

    this.siesta=siesta;
    this.vueltas=vueltas;


    //Inicialización de la hebra.
    thr=new Thread(this, nombre);
  }


  //Método que implementa el cuerpo de ejecuciónd e la hebra.
  public void run(){
    //El código que se ejecuta se hacen dentro de un bloque try-cath para controlar sus excepciones.
    try{
      //Para generar lo snúmeros aleatorios
      Random aleatorio = new Random();

      for(int i=0; i<vueltas || vueltas==0; i++){
        //Imprime el nombre.
        System.out.println("Vuelta no."+i+", de "+thr.getName());
        //Duerme un tiempo aleatorio entre 0 y siesta-1 milisegundos
        if(siesta>0){
          Thread.sleep(aleatorio.nextInt(siesta));
        }
      }
    }catch(InterruptedException e){
      System.out.println(Thread.currentThread().getName()+": me fastidiaron la siesta!");
    }
  }//Fin del método run.
}//Fin de la clase TipoHebra

/*
Clase principal que incluye el main de ejecución.
*/
class main{

  //Esta clase sólo tiene el método main.
  public static void main(String[] args){

    try{
    //Gestionamos que el número de valores pasados sea el correcto.
    if(args.length < 3){
      System.out.println("Error: falta n_hebras, t_max_siesta, vueltas");
      System.exit(1);
    }

    //Capturamos los valores desde los parámetros pasados en la ejecución
    int nHebras=Integer.parseInt(args[0]);
    int siesta=Integer.parseInt(args[1]);
    int vueltas=Integer.parseInt(args[2]);

    //Mostramos por consola el valor de todas estas variables:
    System.out.println("numero de Hebras: "+nHebras+" valor de la siesta: "+siesta+ " vueltas: "+vueltas);


    //Finalmente creamos el vector de hebras y se lanzan todas:

    TipoHebra[] vectorHebras = new TipoHebra[nHebras];

    //Inicializamos los objetos de tipo hebra y los lanzamos.
    for(int i=0; i<nHebras; i++){
      //Se configura su nombre.
      String nombre="Hebra nº. "+i;
      //Se inicializa el objeto.
      vectorHebras[i] = new TipoHebra(nombre,siesta,vueltas);
      //Se lanza la ejecución
      vectorHebras[i].thr.start();
    }

    //La hebra principal duerme durante un segundo
    Thread.sleep(1000);
    System.out.println("main(): he terminado de dormir");

    //Espero a que terminen todas las hebras creadas, haciendo las llamadas a join
    for(int i=0; i<nHebras; i++)
      vectorHebras[i].thr.join();

    }catch(InterruptedException e){
          System.out.println("main(): me fastidiaron la siesta!");
    }
  }//Fin del método main.
}//Fin de la clase main.
