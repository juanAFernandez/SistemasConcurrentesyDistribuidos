import java.util.Random;
import monitor.* ;

//Siempre debemos de hacer una comprobación del problema antes de bloquear o liberar un proceso. !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//##Clase auxiliar para dormir e imprimir.##//
class aux{
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
class Barberia extends AbstractMonitor{

  private Condition colaSalaEspera=makeCondition();
  private Condition colaBarbero=makeCondition();

  //Invocado por los clientes para cortarse el pelo
  public void cortarPelo(){
    enter();
      //Si el barbero está dormido lo despierta:
      if(!colaBarbero.isEmpty())
        colaBarbero.signal();
    leave();
  }

  //Invocado por el barbero para esperar(si procede) a un nuevo cliente y sentarlo para el corte.
  public void siguienteCliente(){
    enter();
      //Si no hay clientes el barbero se duerme:
      if(colaSalaEspera.isEmpty())
        colaBarbero.await();
      else //Se llama (despierta) a un cliente.
      colaSalaEspera.signal();
    leave();

  }

  //Invocado por el barbero para indicar que ha terminado de cortar el pelo.
  public void finCliente(){
    enter();

    leave();
  }



}

class Barbero implements Runnable{

  private Barberia barberia;
  public Thread thr;

  public Barbero(Barberia nuevaBarberia, String nombreHebra){
    barberia=nuevaBarberia;
    thr = new Thread(this, nombreHebra);
  }

  public void run(){
    while(true){
      aux.imprime("Llamando a siguiente cliente");
      barberia.siguienteCliente();
      aux.dormir_max(2500);
      barberia.finCliente();
    }

  }

}

class Cliente implements Runnable{

  private Barberia barberia;
  public Thread thr;

  public Cliente(Barberia nuevaBarberia, String nombreHebra){
    barberia=nuevaBarberia;
    thr = new Thread(this,nombreHebra);
  }

  public void run(){

    while(true){
      barberia.cortarPelo();
      aux.dormir_max(2000);
    }

  }


}






class barberoDurmiente{
  public static void main(String[] args){

    aux.imprime("Barbero Durmiente");

    Barberia barberia = new Barberia();
    Barbero barbero = new Barbero(barberia, "barbero");

    Cliente clienteA = new Cliente(barberia, "clienteA");

    barbero.thr.start();
    clienteA.thr.start();


  }
}
