import monitor.* ;
import java.util.Random ;

class MonitorLE extends AbstractMonitor
{
  //Número de lectores leyendo en ese momento
  private int num_lectores = 0 ;
  private boolean escribiendo = false ;

  private Condition lectura   = makeCondition();
  private Condition escritura = makeCondition();

  public void inicio_lectura()
  { enter();
    if (escribiendo) lectura.await();
    num_lectores++;
    lectura.signal();
    leave();
  }
  public void fin_lectura()
  { enter();
    num_lectores--;
    if (num_lectores==0)
     //Se hace un signal a la cola escritura para que los escritores que haya, se libere uno, el que más tiempo lleva.
     escritura.signal();
    leave();
  }
  public void inicio_escritura()
  {
    enter(); //ente() y leave() se usan para la exclusión mutua
    //Si hay lectores leyendo o escritores escribiendo el escribor espera.
    if (num_lectores>0 || escribiendo)
      //escritura es una cola y el proceso que se va a quedar pendiente ahí hasta que se haga un .signal() por un fin_lectura o fin_escritura.
      escritura.await();

    //Si la condición anterior no se da el escritor se pone a escribir.
    escribiendo=true;

    leave();
  }
  public void fin_escritura() // prio. lect
  { enter();
    escribiendo=false;
    if (lectura.isEmpty()) escritura.signal();
    else lectura.signal();
    leave();
  }
} // fin clase monitor "Lect\_Esc"

class aux
{
  static Random genAlea = new Random() ;
  static void dormir_max( int milisecsMax )
  { try
    { Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    }
    catch( InterruptedException e )
    { System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}

class Lector implements Runnable
{
  private MonitorLE monitorLE ; // objeto monitor l.e. compartido
  private int       nveces ; // numero de veces que lee
  public Thread     thr   ; // objeto hebra encapsulado

  public Lector( MonitorLE p_monitorLE, int p_nveces, String nombre )
  { monitorLE  = p_monitorLE  ;
    nveces     = p_nveces ;
    thr        = new Thread(this,nombre);
  }
  public void run()
  { for( int i = 0 ; i < nveces ; i++ )
    { System.out.println( thr.getName()+": solicita lectura.");
      monitorLE.inicio_lectura();
        System.out.println( thr.getName()+": leyendo.");
        aux.dormir_max( 1000 ) ;
      monitorLE.fin_lectura();
    }
  }
}

class Escritor implements Runnable
{
  private MonitorLE monitorLE ; // objeto monitor l.e. compartido
  private int       nveces ; // numero de veces que lee
  public Thread     thr   ;  // objeto hebra encapsulado

  public Escritor( MonitorLE p_monitorLE, int p_nveces, String nombre )
  { monitorLE  = p_monitorLE  ;
    nveces     = p_nveces ;
    thr        = new Thread(this,nombre);
  }
  public void run()
  {
    for( int i = 0 ; i < nveces ; i++ )
    { System.out.println( thr.getName()+": solicita escritura.");
     //Llamada a un método del monitor
      monitorLE.inicio_escritura();

        System.out.println( thr.getName()+": escribiendo.");
        aux.dormir_max( 1000 );
      monitorLE.fin_escritura ();
    }
  }
}

class EjemploLectorEscritor
{ public static void main(String[] args)
  { if ( args.length != 4 )
    { System.err.println("Uso: num_lectores num_escritores num_iters_lector num_iters_escritor");
        return ;
    }
    // leer parametros, crear vectores, crear monitor
    Lector[]   vlec = new Lector[ Integer.parseInt(args[0]) ]; //Hebras lectoras
    Escritor[] vesc = new Escritor[ Integer.parseInt(args[1]) ]; //Hebras escritoras
    int iter_lector = Integer.parseInt(args[2]);
    int iter_escritor = Integer.parseInt(args[3]);
    MonitorLE monitor = new MonitorLE();

    // crear hebras lectoras
    for( int i = 0; i < vlec.length; i++)
      vlec[i] = new Lector(monitor, iter_lector,"lector"+(i+1));

    // crear hebras escritoras
    for( int i = 0; i < vesc.length; i++)
      vesc[i] = new Escritor(monitor, iter_lector,"escritor"+(i+1));

    // lanzar hebras
    for( int i = 0; i < vlec.length ; i++) vlec[i].thr.start();
    for( int i = 0; i < vesc.length ; i++) vesc[i].thr.start();
  }
}
