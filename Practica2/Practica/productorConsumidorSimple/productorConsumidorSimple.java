
import monitor.*;
import java.util.Random;


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



class Monitor extends AbstractMonitor{

  private int contenedorUnitario;
  private boolean estaVacio;
  private Condition puedeProducir=makeCondition();
  private Condition puedeConsumir=makeCondition();

  public Monitor(){
    contenedorUnitario=0;
    //El monitor comienza con el contenedor vacío.
    estaVacio=true;
  }

  public void producir(int n){
    enter();
      if(estaVacio){
        contenedorUnitario=n;
        aux.imprimir("Produzco "+n);
        estaVacio=false;
        puedeConsumir.signal();
      }
      if(!estaVacio){
        puedeProducir.await();
      }

    leave();
  }

  public void consumir(){
    enter();
      if(estaVacio){
        puedeConsumir.await();
      }
      if(!estaVacio){
        aux.imprimir("Consumo "+contenedorUnitario);
        aux.dormir_max(2000);
        estaVacio=true;
        puedeProducir.signal();
      }

    leave();
  }

}

class Productor implements Runnable{

  private Monitor monitor;
  public Thread hebra;
  int valor;

  public Productor(Monitor monitorPasado, String nombreHebra){
    monitor=monitorPasado;
    hebra=new Thread(this,nombreHebra);
  }

  //Núcleo de ejecución de la hebra productora:
  public void run(){
    while(true){
      /*La producción del valor no tiene por qué inferir en el consumo del
      mismo, puede estar consumiendose uno mientras se está produciendo
      el otro. Por eso no se introduce en el proceso del monitor.*/
      valor=aux.producirAleatorio();
      aux.dormir_max(2000);
      monitor.producir(valor);
    }
  }

}


class Consumidor implements Runnable{

  private Monitor monitor;
  public Thread hebra;

  public Consumidor(Monitor monitorPasado, String nombreHebra){
    monitor=monitorPasado;
    hebra=new Thread(this,nombreHebra);
  }

  //Núcleo de ejecución de la hebra consumidora:
  public void run(){
    while(true){
      monitor.consumir();
    }

  }

}


//Programa principal
class productorConsumidorSimple{
  public static void main(String[] args){

    Monitor monitor = new Monitor();
    Productor productor = new Productor(monitor, "produtor");
    Consumidor consumidor = new Consumidor(monitor, "consumidor");

    //Lanzamiento de ejecución de las hebras:
    productor.hebra.start();
    consumidor.hebra.start();


  }
}
