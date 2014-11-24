import java.util.Random;
import monitor.* ;

//##Clase auxiliar para dormir e imprimir.##//
class aux
{
  static Random genAlea = new Random() ;
  static void dormir_max( int milisecsMax ){
    try{
      Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    }catch( InterruptedException e ){
       System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
  static void imprime(String mensaje){
    System.out.println(mensaje);
  }
}

//##MONITOR##//
class SalaEspera extends AbstractMonitor{


  //Cola de procesos esperando en la sala de espera.
  private Condition colaSalaEspera=makeCondition();


//##METODOS DEL MONITOR##//
}

class Barbero implements Runnable{

  private SalaEspera salaEspera;
  public Thread thr;

  public Barbero(SalaEspera salaPasada, String nombreHebra){
    salaEspera=salaPasada;
    thr = new Thread(this, nombreHebra);
  }

  public void run(){

  }

}

class Cliente implements Runnable{

  private SalaEspera salaEspera;
  public Thread thr;

  public Cliente(SalaEspera salaPasada, String nombreHebra){
    salaEspera=salaPasada;
    thr = new Thread(this,nombreHebra);
  }

  public void run(){

  }


}






class barberoDurmiente{
  public static void main(String[] args){

    aux.imprime("Barbero Durmiente");

    SalaEspera salaEspera = new SalaEspera();
    Barbero barbero = new Barbero(salaEspera, "barbero");

    Cliente clienteA = new Cliente(salaEspera, "clienteA");

    barbero.thr.start();
    clienteA.thr.start();


  }
}
