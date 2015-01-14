/**
* En esta segunda implementación del programa de los fumadores vamos a usar tres colas condionales
* en vez de una. En cada cola quedarán encolados los procesos fumadores del mismo tipo.
* En este caso cada fumador estará en cada cola, un fumador por cada cola de condicion.
*
*/

import monitor.* ;
import java.util.Random ;

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

//MONITOR ESTANCO (recurso compartido por las hebras)
class Mostrador extends AbstractMonitor{

  private int ingredienteMesa;
  private boolean hayIngrediente=false;

  private Condition colaFumadoresFaltaTabaco = makeCondition();
  private Condition colaFumadoresFaltaPapel = makeCondition();
  private Condition colaFumadoresFaltaCerillas = makeCondition();



  private Condition puedeServir = makeCondition();


  // ## CONDICIONES ##

  //#1 Condición de ingrediente en mesa.



  //Constructor del objeto comparido mostrador:
  public Mostrador(){
    //Al principio no hay ningún ingrediente.
    ingredienteMesa=0;



  }

  //Método invocado por los fumadores que quieren coger un ingrediente.
  public void obtenerIngrediente (String nombreFumador, int miIngrediente){
    enter();

      System.out.println("Soy "+nombreFumador+" y voy a ver si este es mi ingrediente");

      /*Si no hay ingrediente en la mesa, el fumador que invoca a este
      método se pone a la espera de la condición "puedeCoger". Se mete en
      la cola y espera a que lo dejen salir.  */
      if(!hayIngrediente){
        //Si no ha ingrediente espero.
        //SOL [A]. En esta primera solución sólo hay una cola y los fumadores entrarán por defecto
        //en ella hasta que el estanquero empiece a trabajar.
        System.out.print("Soy "+nombreFumador+" y voy a esperar ");


        //Me pongo en la cola que me corresponde. El proceso que invoca a await queda encolado en esa cola.

        if(miIngrediente==1){
          System.out.println(" en la cola del papel.");
          colaFumadoresFaltaPapel.await();
        }
        if(miIngrediente==2){
          System.out.println(" en la cola del tabaco.");
          colaFumadoresFaltaTabaco.await();
        }
        if(miIngrediente==3){
          System.out.println(" en la cola de las cerillas.");
          colaFumadoresFaltaCerillas.await();
        }

      }else{ //Hay un ingrediente en la mesa


        //Si hay ingrediente y es el que yo necesito me lo llevo y fumo.
        if(miIngrediente==ingredienteMesa){
          //Acción de coger el ingrediente
          hayIngrediente=false;
          System.out.println("Soy "+nombreFumador+ "y recogo ingrdiente");

          //Al no haber ingrediente el estanquero puede seguir trabajando
          //Despierto al estanquero para que pueda seguir funcionando.
          puedeServir.signal();
        }

        /*Tras recoger el ingrediente (no me he quedado bloqueado) puedo finalizar la ejecución de esta función
          y volver al run().
        */



      }



    leave();
  }
  //Invocado por el estanquero, indicando el ingrediente que pone
  public void ponerIngrediente(int ingrediente){

    enter();
      if(!hayIngrediente){

        // ### PRIMERO LO PONGO ###

        ingredienteMesa=ingrediente;
        System.out.print("El estanquero coloca");
        if(ingredienteMesa==1)
          System.out.println(" papel");
        if(ingredienteMesa==2)
          System.out.println(" tabaco");
        if(ingredienteMesa==3)
          System.out.println(" cerillas");
        //Establezco que hay ingrediente.
        aux.dormir_max(2000);
        hayIngrediente=true;


      // ### DESPUÉS DESPIERTO ### Al fumador que esté en la cola de ese ingrediente.
      if(ingredienteMesa==1)
        colaFumadoresFaltaPapel.signal();
      if(ingredienteMesa==2)
        colaFumadoresFaltaTabaco.signal();
      if(ingredienteMesa==3)
        colaFumadoresFaltaCerillas.signal();

      //y acabo el proceso de ponerIngrediente();
    }else{

      //Si ya hay ingrediente:
      puedeServir.await();

    }


    leave();

  }

  //Invocado por el estanquero
  public void llamarFumador(){
    enter(); //Asegura E.M.



    //Si hay ingrediente en el mostrador mando una señal para que depierte el fumador que está en la cola de
    //ese tipo de ingrdiente.
    if(hayIngrediente){

      //Después de poner el ingrediente tengo que llamar al fumador correspondiente.
      System.out.print("Llamando a un fumador de la cola ");


      if(ingredienteMesa==1){
        colaFumadoresFaltaPapel.signal();
        System.out.println(" de los que necesitan papel.");
      }
      if(ingredienteMesa==2){
        colaFumadoresFaltaTabaco.signal();
        System.out.println(" de los que necesitan tabaco.");
      }
      if(ingredienteMesa==3){
        colaFumadoresFaltaCerillas.signal();
        System.out.println(" de los que necesitan cerillas.");
      }

      aux.dormir_max(2000);
    }


    leave(); //Asegura E.M.
  }



  //Invocado por el estanquero
  public void esperarRecogidaIngrediente(){

  }

}//Fin del monitor ESTANCO


//Hebra FUMADOR
class Fumador implements Runnable{

  //Atributos de la clase:

  String nombreFumador;
  private Mostrador mostrador; /**< Objeto de tipo Estanco, compartido por todas las hebras.  */
  int ingredienteNecesario; /**< Ingrediente que el fumador en concreto necesita. Dado en el constructor.*/
  public Thread thr; /** < Objeto de tipo Thread encapsulado necesario para que la hebra se ejecute. Que debe ser público para que
  desde el main pueda lanzarse la hebra. */


  /**
  * @brief Constructor de la clase Fumador
  * @param estancoPasado El objeto que de tipo Estanco que compartirán todas las hebras.
  * @param nombre Nombre de la hebra que sirve para el constructor de Thread (para mensajes).
  */
  public Fumador( Mostrador mostradorPasado, String nombre, String ingredientePasado ){

    nombreFumador=nombre;

    //Asignamos el objeto compartido para acceder a el.
    mostrador = mostradorPasado;

    /*Iniciamos el tipo Thread con el nombre de la hebra y con este objeto para que así
    se ejecute el método run() cuando se invoque la llamada desde el main() */
    thr = new Thread(this,nombre);

    //Vemos el tipo de ingrediente que el fumador necesita a partir del string pasado:
    if(ingredientePasado=="papel")
      ingredienteNecesario=1;
    if(ingredientePasado=="tabaco")
      ingredienteNecesario=2;
    if(ingredientePasado=="cerillas")
      ingredienteNecesario=3;

    //Mensaje de presentación:
    System.out.println("Soy "+nombre+" y me falta "+ingredientePasado);


  }



  //Cuerpo de ejecución de la hebra:
  public void run(){



    while(true){
      //Obtener el ingrediente del mostrador (objeto compartido);
      mostrador.obtenerIngrediente(nombreFumador, ingredienteNecesario);
      //Acción de fumar

      aux.dormir_max(20000);
    }

  }


}//Fin de la clase de la hebra FUMADOR

//Hebra ESTANQUERO
class Estanquero implements Runnable{

  //Declaración del objeto tipo mostrador.
  private Mostrador mostrador;

  public Thread thr;


  public Estanquero(Mostrador mostradorPasado, String nombre){
    //Asignamos el objeto compartido al nuestro para poder acceder a el.
    mostrador=mostradorPasado;

    /*Iniciamos el tipo Thread con el nombre de la hebra y con este objeto para que así
    se ejecute el método run() cuando se invoque la llamada desde el main() */
    thr = new Thread(this,nombre);
  }


  public void run(){

    //Se declara un entero que representará un ingrediente.
    int ingrediente;

    //La ejecución del cuerpo es infinita.
    while(true){
      //#1 Se genera el ingrediente de forma aleatoria:
      ingrediente=(int)(Math.random()*3+1);
      //#2 Se usa el mostrador compartido para poner el ingrediente:
      mostrador.ponerIngrediente(ingrediente);

      mostrador.llamarFumador();

      //#3 Se espera para que se pueda volver a poner ingredientes:
      //mostrador.esperarRecogidaIngrediente();

    }

  }
}//Fin de la clase de la hebra ESTANQUERO




class fumadores2{
  public static void main(String[] args){
    System.out.println("Fumadores on fire!");

    //Objeto del tipo monitor, que será compartido por todas las hebras.
    Mostrador mostrador = new Mostrador();

    //Declaramos e inicializamos todos los objetos y les pasamos el objeto compartido monitor.
    Estanquero estanquero = new Estanquero(mostrador, "estanquero");
    System.out.println("Inicialización de los fumadores");
    Fumador fumadorA = new Fumador(mostrador, "fumadorA", "papel");
    Fumador fumadorB = new Fumador(mostrador, "fumadorB", "tabaco");
    Fumador fumadorC = new Fumador(mostrador, "fumadorC", "cerillas");
    System.out.println("");
    //Lanzamos la hebras haciendo uso del objeto tipo Thread que cada objeto tiene en su interior.
    estanquero.thr.start();
    fumadorA.thr.start();
    fumadorB.thr.start();
    fumadorC.thr.start();


  }
}
