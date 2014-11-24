#Seminario 2

###Hebras en Java

Para poder usar hebras en java tenemos que definir la implementación de la interfaz Runnable (como alternativa a heredar de la clase Thread, que descartamos aunque sea más sencilla por ser menos flexible).

Centrándonos en lo básico, si queremos tener varias hebras que ejecutan el mismo código debemos implementar ese código dentro del método **run()** de una clase que implementa **runable**, como:

```sh
class TipoHebra implements Runnable{

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
```

Una vez que tenemos eso podemos construir un main desde el que creamos tantos objetos del tipo TipoHebra como hebras queramos ejecutar, inicializando cada uno de estos como queramos. Una vez inicializadas podemos lanzar las hebras a ejecución (sólo se ejecutará el código encerrado en el **run**) con **.thr.start()** y obligamos a que acaben antes de acabar el programa general con **thr.join()**, aunque en nuestro caso tendremos que forzar el cierre por tener en run un bucle infinito.

```sh
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
```


La ejecución del código daría como resultado esta salida:

```sh
Ubuntu> javac ejemplo1.java
Ubuntu> java main 500
Hola, soy hebra 1 y me voy a dormir.
Hola, soy hebra 2 y me voy a dormir.
Hola, soy hebra 1 y me voy a dormir.
Hola, soy hebra 2 y me voy a dormir.
Hola, soy hebra 1 y me voy a dormir.
Hola, soy hebra 2 y me voy a dormir.
Hola, soy hebra 1 y me voy a dormir.
Hola, soy hebra 2 y me voy a dormir.
Hola, soy hebra 1 y me voy a dormir.
Hola, soy hebra 2 y me voy a dormir.
```
Bastante fácil.

En Java las hebras se ejecutan según su prioridad,
podemos ampliar el código para conocerla:

```sh
hebra1.thr.start();
System.out.println("Prioridad de la hebra 1: "+hebra1.thr.getPriority());
hebra2.thr.start();
System.out.println("Prioridad de la hebra 2: "+hebra2.thr.getPriority());
```
###Vector de Hebras y Random:


En el código de ejemplo2.java se puede ver como implementar la misma idea pero con un vector de hebras, donde se lanzar mediante un bucle y se esperan con otro:

extracto:
```sh
//Inicializamos los objetos de tipo hebra y los lanzamos.
for(int i=0; i<nHebras; i++){
  //Se configura su nombre.
  String nombre="Hebra nº. "+i;
  //Se inicializa el objeto.
  vectorHebras[i] = new TipoHebra(nombre,siesta,vueltas);
  //Se lanza la ejecución
  vectorHebras[i].thr.start();
}
```

extracto:
```sh
//Espero a que terminen todas las hebras creadas, haciendo las llamadas a join
for(int i=0; i<nHebras; i++)
  vectorHebras[i].thr.join();
```

###Interacción entre hebras con objetos compartidos.
####Calculo de múltiplos
