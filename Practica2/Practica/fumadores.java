import monitor.* ;
import java.util.Random ;

//MONITOR ESTANCO (recurso compartido por las hebras)
class Mostrador extends AbstractMonitor{

  private int ingrediente;

  //Constructor del objeto comparido mostrador:
  public Mostrador(){
    //Al principio no hay ningún ingrediente.
    ingrediente=0;
  }

  //Invocada por cada fumador, indicando su ingrediente o numero
  public void obtenerIngrediente (int miIngrediente){
      System.out.println("Ob");
  }
  //Invocado por el estanquero, indicando el ingrediente que pone
  public void ponerIngrediente(int ingrediente){

  }
  //Invocado por el estanquero
  public void esperarRecogidaIngrediente(){

  }

}//Fin del monitor ESTANCO


//Hebra FUMADOR
class Fumador implements Runnable{

  //Atributos de la clase:

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
    //Asignamos el objeto compartido para acceder a el.
    mostrador = mostradorPasado;

    /*Iniciamos el tipo Thread con el nombre de la hebra y con este objeto para que así
    se ejecute el método run() cuando se invoque la llamada desde el main() */
    thr = new Thread(this,nombre);

    //Vemos el tipo de ingrediente que el fumador necesita a partir del string pasado:
    if(ingredientePasado=="papel")
      ingredienteNecesario=0;
    if(ingredientePasado=="tabaco")
      ingredienteNecesario=1;
    if(ingredientePasado=="cerillas")
      ingredienteNecesario=2;
  }



  //Cuerpo de ejecución de la hebra:
  public void run(){



    while(true){
      mostrador.obtenerIngrediente(ingredienteNecesario);
      //aux.dormir_max(2000);
    }

  }


}//Fin de la clase de la hebra FUMADOR

//Hebra ESTANQUERO
class Estanquero implements Runnable{

  //Declaración del objeto tipo mostrador.
  private Mostrador mostrador();

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
      ingrediente=(int)(Math.random()*0.3);
      //#2 Se usa el mostrador compartido para poner el ingrediente:
      mostrador.ponerIngrediente(ingrediente);
      //#3 Se espera para que se pueda volver a poner ingredientes:
      mostrador.esperarRecogidaIngrediente();

    }

  }
}//Fin de la clase de la hebra ESTANQUERO




class fumadores{
  public static void main(String[] args){
    System.out.println("Fumadores on fire!");

    //Objeto del tipo monitor, que será compartido por todas las hebras.
    Mostrador mostrador = new Mostrador();

    //Declaramos e inicializamos todos los objetos y les pasamos el objeto compartido monitor.
    Estanquero estanquero = new Estanquero(estanco);
    Fumador fumadorA = new Fumador(estanco, "fumador");
    Fumador fumadorB = new Fumador(estanco, "papel");
    Fumador fumadorC = new Fumador(estanco, "cerillas");

    //Lanzamos la hebras haciendo uso del objeto tipo Thread que cada objeto tiene en su interior.
    fumadorA.thr.start();

  }
}
