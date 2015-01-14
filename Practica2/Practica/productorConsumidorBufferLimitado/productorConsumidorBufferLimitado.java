import monitor.*;
import java.util.Random;

/*
About:
  Implementación de un buffer con una pila LIFO (Last Input First Output) donde el último
  elemento que entra es el primero que sale.
*/


class aux{
  static Random genAlea = new Random();
  static void dormir_max(int milisecsMax){
    try{
      Thread.sleep(genAlea.nextInt(milisecsMax));
    }catch(InterruptedException e){
      System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
  static void imprimir(String mensaje){
    System.out.println(mensaje);
  }
  static int producirAleatorio(){
    return (int)(Math.random()*5+1);
  }
}



class Buffer extends AbstractMonitor{

  //Buffer
  private int[] buffer;

  //Tamaño del buffer:
  //private int numElementos;

  //Posición de ocupación del buffer:
  private int posBuffer;

  private boolean puedeProducir;
  private boolean puedeConsumir;


  private Condition colaProductores  = makeCondition();
  private Condition colaConsumidores = makeCondition();


  public Buffer(int tamBuffer){


    buffer = new int[tamBuffer];

    aux.imprimir("Inicializado buffer con tamaño "+buffer.length);

    //Estas dos variables podrían ser las mismas!!!, pero bueno... :/
    //numElementos=0;
    posBuffer=-1;
    //El monitor comienza con el contenedor vacío.
    //estaVacio=true;

    //El buffer comienza vacío y por eso se pueden producir valores en el.
    puedeProducir=true;

    //AL comenzar vacío no se puede consumir de el:
    puedeConsumir=false;

  }

  //Introducir datos en el buffer
  public void producir(String nombreHebra, int n){
    enter();

      //Si el buffer está lleno, no se pueden introducir más valores:
      if(!puedeProducir){
        //No puedo producir más, me coloco en la cola de productores a dormir.
        colaProductores.await();
      }

      //Si el buffer no está lleno:
      if(puedeProducir){

        //Hacemos que avance el buffer:
        posBuffer++;

        //Escribimos en la posición del buffer donde lo indique posBuffer:
        buffer[posBuffer]=n;

        //Si el buffer está lleno ya no se puede producir más:
        if(posBuffer==(buffer.length-1)){
          aux.imprimir(nombreHebra+" ya no puede producir más");
          puedeProducir=false;
        }

        //Mostramos un mensaje por pantalla:
        aux.imprimir(nombreHebra+" produce "+n+" en pos "+posBuffer);
        //Como hemos producido un valor despertamos a los hipotéticos consumidores.
        colaConsumidores.signal(); puedeConsumir=true;
      }

    leave();
  }

  public void consumir(String nombreHebra){
    enter();

      //Si en el buffer hay elementos, se pueden consumir:
      if(puedeConsumir){
        aux.imprimir(nombreHebra+" consume "+buffer[posBuffer]+" pos: "+posBuffer);
        //Retrasamos la posición del buffer
        posBuffer--;

        //Si el buffer se ha vaciado ya no se puede consumir más.
        if(posBuffer==-1)
          puedeConsumir=false;

        aux.dormir_max(2000);
        puedeProducir=true;
      }

      //Si no hay elementos en el buffer, me duermo.
      if(!puedeConsumir){
        aux.imprimir(nombreHebra+" se va a dormir");
        colaConsumidores.await();
      }


    leave();
  }

}

class Productor implements Runnable{

  private Buffer buffer;
  private String nombreHebra;
  public Thread hebra;
  int valor;

  public Productor(Buffer bufferPasado, String nuevoNombre){
    nombreHebra=nuevoNombre;
    buffer=bufferPasado;
    hebra=new Thread(this,nombreHebra);
    aux.imprimir(nombreHebra+ "inicializado.");
  }

  //Núcleo de ejecución de la hebra productora:
  public void run(){
    while(true){
      /*La producción del valor no tiene por qué inferir en el consumo del
      mismo, puede estar consumiendose uno mientras se está produciendo
      el otro. Por eso no se introduce en el proceso del monitor.*/
      valor=aux.producirAleatorio();
      aux.dormir_max(2000);
      buffer.producir(nombreHebra, valor);
    }
  }

}


class Consumidor implements Runnable{

  private Buffer buffer;
  private String nombreHebra;
  public Thread hebra;


  //Constructor del objeto Consumidor
  public Consumidor(Buffer bufferPasado, String nuevoNombre){
    buffer=bufferPasado;
    nombreHebra=nuevoNombre;
    hebra=new Thread(this,nombreHebra);
    aux.imprimir(nombreHebra+ " inicializado.");
  }

  //Núcleo de ejecución de la hebra consumidora:
  public void run(){
    while(true){
      buffer.consumir(nombreHebra);
    }

  }

}


//Programa principal
class productorConsumidorBufferLimitado{
  public static void main(String[] args){

    //Comprobamos el número de parámetros pasados al programa:
    if(args.length != 3){
      System.out.println("Faltan parámetros. Uso: numProductores, numConsumidores, tamBuffer");
      return;
    }

    //Recogemos los valores de pasada al programa
    int numProductores = Integer.parseInt(args[0]);
    int numConsumidores = Integer.parseInt(args[1]);
    int tamBuffer = Integer.parseInt(args[2]);

    //Mensaje de bienvenida:
    aux.imprimir("Productor-Consumidor con "+numProductores+" productores, "+numConsumidores+" consumidores y un buffer de "+tamBuffer+" unidades.");

    //Declaramos e inicializamos el buffer (objeto compartido tipo monitor) con el tamaño pasado:
    Buffer buffer = new Buffer(tamBuffer);

    //Declaración del vector de hebras productoras y consumidoras:
    Productor[] productores = new Productor[numProductores];
    Consumidor[] consumidores = new Consumidor[numConsumidores];

    //Inicialización de las hebras productores:
    for(int i=0; i<productores.length; i++)
      productores[i] = new Productor(buffer, "Productor"+i);

    //Inicialización de las hebras consumidoras:
    for(int i=0; i<consumidores.length; i++)
      consumidores[i] = new Consumidor(buffer, "Consumidor"+i);

    //Lanzamiento de ejecución de las hebras productoras y consumidoras:
    for(int i=0; i<productores.length; i++)
      productores[i].hebra.start();
    for(int i=0; i<consumidores.length; i++)
      consumidores[i].hebra.start();

  }
}
