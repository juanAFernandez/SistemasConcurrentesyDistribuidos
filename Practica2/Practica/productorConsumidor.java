import monitor.*;
import java.util.Random;


class MonitorLE extends AbstractMonitor{

  //Variables permanentes
  private int num_lectores=0;

  private boolean escribiendo=false;

  /*Variables condición, representan una cola
  inicialmente vacía de procesos esperando
  la condición.
  */
  private Condition condicionLectura=makeCondition();
  private Condition condicionEscritura=makeCondition();

  /*
  Mas de un proceso podrá estar dentro del monitor, aunque sólo uno
  estará ejecutándose (el resto estarán bloqueados en variables condición).

  await

  signal

  isEmpty

  */

  public void inicio_lectura(){
    enter();
    /*Si se está escribiendo en ese momento.
    */
    if(escribiendo)
      //La lectura debe esperar
      condicionLectura.await();
    num_lectores++;
    condicionLectura.signal();
    leave();
  }

  public void fin_lectura(){
    enter();
    num_lectores--;
    if(num_lectores==0)
      condicionEscritura.signal();
    leave();
  }

  public void inicio_escritura(){
    enter();
    if(num_lectores>0 || escribiendo)
      condicionEscritura.await();
    escribiendo=true;
    leave();
  }

  public void fin_escritura(){
    enter();
    escribiendo=false;
    if(condicionLectura.isEmpty())
      condicionEscritura.signal();
    else
      condicionLectura.signal();
    leave();
  }
}

class Lector implements Runnable{

  private MonitorLE monitorLE;
  private int nveces;
  public Thread thr;

  public Lector (MonitorLE p_monitorLE, int p_nveces, String nombre){
    monitorLE=p_monitorLE;
    nveces=p_nveces;
    thr=new Thread(this,nombre);
  }

  public void run(){
    for(int i=0; i<nveces; i++){
      System.out.println(thr.getName()+": solicita lectura.");
      monitorLE.inicio_lectura();
      System.out.println(thr.getName()+": leyendo.");
      aux.dormir_max(1000);
      monitorLE.fin_lectura();
    }
  }

}


class Escritor implements Runnable{

  private MonitorLE monitorLE;
  private int nveces;
  public Thread thr;

  public Escritor(MonitorLE p_monitorLE, int p_nveces, String nombre){
    monitorLE=p_monitorLE;
    nveces=p_nveces;
    thr=new Thread(this,nombre);
  }

  public void run(){
    for(int i=0; i<nveces; i++){
      System.out.println(thr.getName()+": solicita escritura.");
      monitorLE.inicio_escritura();
      System.out.println(thr.getName()+": escribiendo");
      aux.dormir_max(1000);
      monitorLE.fin_escritura();
    }
  }
}


class aux{
  static Random genAlea = new Random();
  static void dormir_max(int milisecsMax){
    try{
      Thread.sleep(genAlea.nextInt(milisecsMax));
    }catch(InterruptedException e){
      System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}











class productorConsumidor{
  public static void main(String[] args){



  }
}
