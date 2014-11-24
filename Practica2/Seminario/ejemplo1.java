/*
  Clase Java que implementa una hebra, para esto necesitamos implementar el
  método run. Además esta clase debe implementar la interfaz Runnable para ser
  una hebra.
*/
class TipoHebra implements Runnable{

  //Nombre de la hebra
  String nombre;
  //Tiempo que duerme la hebra
  long siesta;
  //Objeto de tipo hebra encapsulado
  public Thread thr;

  //Constructor de la clase
  public TipoHebra(String nombre, long siesta){
    this.siesta=siesta;
    thr=new Thread(this, nombre);
  }
  //Método que implementa el cuerpo de ejecuciónd e la hebra.
  public void run(){
    //El código que se ejecuta se hacen dentro de un bloque try-cath para controlar sus excepciones.
    try{
      //La hebra ejecuta un bucle que lo único que hace es saludar y dormir.
      while(true){
        System.out.println("Hola, soy "+thr.getName()+" y me voy a dormir.");
        if(siesta>0)
          Thread.sleep(siesta);
      }
    }catch(InterruptedException e){
      System.out.println("me fastidiaron la siesta!");
    }
  }
}

/*
Clase principal que incluye el main de ejecución.
*/

class main{

  //Esta clase sólo tiene el método main.
  public static void main(String[] args) throws InterruptedException{
    //Gestionamos que el número de valores pasados sea el correcto.
    if(args.length < 1){
      System.out.println("Error: falta valor de la siesta");
      System.exit(1);
    }

    //Capturamos el valor desde el parámetro pasado en la ejecución
    long siesta=Long.parseLong(args[0]);
    /*Creamos un tipo de objeto del TDA creado anteriormente y lo
    inicializamos con el los parametros que requiere su constructor.*/
    TipoHebra hebra1 = new TipoHebra("hebra 1", siesta);
    TipoHebra hebra2 = new TipoHebra("hebra 2", siesta);

    //Lanzamos la hebra, en realidad sólo el cuerpo, la función run.
    hebra1.thr.start();
    hebra2.thr.start();
    //Hacemos que la hebra principal duerma de 1 a 10 segundos.
    Thread.sleep(100);
    //Esperamos a que nuestra hebra termine con la operación join clásica.
    hebra2.thr.join();
    hebra1.thr.join();

  }//Fin del método main.
}
