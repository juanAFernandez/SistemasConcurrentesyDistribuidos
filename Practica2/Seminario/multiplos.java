/*
* Fuente de Java donde vamos a usar dos hebras que accederan a un objeto compartido para
* calcular los múltiplos de 2 y de 3 desde 1 a 1000 ambos inclusive. Para conseguir esto
* se hará uso de synchronized de Java.
*
*/

/* Las hebras pueden compartir objetos, estos deben ser instancias de una clase. En nuestro
* caso el objeto compartido es un contador.
*
*/

class Contador{

  private long valor;

  public Contador(long inicial){
    valor=inicial;
  }

  void retrasoOcupado(){
    long tmp=0;
    for(int i=0; i<100000; i++)
      tmp=tmp*i-tmp-i;
  }

  public synchronized void incrementa(){
    long aux =valor;
    retrasoOcupado();
    valor=aux+1;
  }

  public synchronized long getValor(){ return valor;}

}


/*
* Definimos la clase de la hebra que como siempre implementa la interfaz Runnable
*/

class Hebra implements Runnable{
  int numero;
  public Thread thr;
  private Contador cont;

  public Hebra (String nombre, int numero, Contador contador){
    thr = new Thread(nombre);
    this.numero=numero;
    this.cont=contador;
  }

  public void run(){
    for(int i=0; i<=1000; i++){
      if(i%numero==0)
        cont.incrementa();
    }
  }

}






class main{

  public static void main(String[] args){
    try{

      //Creamos un objeto de tipo contador al que podrán acceder las dos hebras.
      Contador contH = new Contador(0);
      //Declaramos un vector de hebras que contendŕa las dos que necesitamos
      Hebra[] vectorHebras= new Hebra[2];
      vectorHebras[0] = new Hebra( "Contador multiplos de 2 ", 2, contH);
      vectorHebras[1] = new Hebra( "Contador mulitplos de 3 ", 3, contH);

      //Lanzamos las hebras:
      vectorHebras[0].thr.start();
      vectorHebras[1].thr.start();

      //Esperamos terminación de las hebras:
      vectorHebras[0].thr.join();
      vectorHebras[1].thr.join();

      System.out.println("Resultado hebras : "+contH.getValor());
      long contV = 0 ; // contador de verificacion
      for ( int i = 1 ; i <= 1000 ; i++ ){
        if ( i % 2 == 0 ) contV = contV + 1 ;
        if ( i % 3 == 0 ) contV = contV + 1 ;
      }

      System.out.println("Resultado correcto: " + contV);

    }catch(InterruptedException e){
      System.out.println("ha ocurrido una excepción");
    }

  }//Fin del método main()
}//Fin de la clase main
