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


class Estanco extends AbstractMonitor
{
   Condition colaEstanquero ; // cola donde espera el estanquero
                              // hasta que el mostrador queda libre

   int       Fumador ; // identificador del fumador que necesita el
                         // ingredientes que hay puesto en el mostrador,
                         // o -1 si no hay ningún ingrediente puesto

   private Condition[] colaFumador ; // colas donde esperan cada uno de
                             // los tres fumadores

   public Estanco()
   {
	   Fumador      = -1 ; // no hay ingrediente en el mostrador
	   colaEstanquero = makeCondition();
	   colaFumador    = new Condition[3] ;
	   colaFumador[0] = makeCondition();
	   colaFumador[1] = makeCondition();
	   colaFumador[2] = makeCondition();

   }


   //-------------------------------------------------------------------
   // invocado por los fumadores para esperar y obtener sus ingredientes

   public void obtenerIngredientes( int miIngrediente )
   {

      enter() ;

      System.out.print("Fumador "+miIngrediente+" va al mostrador a por ingredientes ....");

      if ( Fumador != miIngrediente )
      {
		 System.out.println(": no están, debe esperar.");
		 colaFumador[miIngrediente].await() ;
      }

       else{
              System.out.println("");

      System.out.println("Fumador "+miIngrediente+" ha recogido sus ingredientes.");
      //Después de recoger el ingrediente se indica.
      Fumador= -1;

      //Y se despierta al estanquero:
      colaEstanquero.signal();
      //colaFumador[Fumador].await() ;



      }

      leave() ;
   }

   //-------------------------------------------------------------------
   // invocado por el estanquero para poner ingredientes en el mostrador,

   public void ponerIngredientes( int fumador_sig)
   {

      enter() ;
       Fumador= fumador_sig;
       System.out.println("Estanquero pone ingrediente para fumador "+Fumador+".");
	     colaFumador[Fumador].signal() ;
       System.out.println("Se ha despertado a fumador "+Fumador);

      leave() ;
   }

   //-------------------------------------------------------------------
   // invocado por el estanquero para esperar a que se recogan los ingredientes

   public void esperarRecogidaIngredientes()
   {
      enter() ;

      if ( Fumador == -1 )
      {
		  System.out.println("Ingredientes no recogidos. Estanquero debe esperar.");
		  colaEstanquero.await() ;
	  }

      leave() ;
   }
}

//**********************************************************************

class Fumador implements Runnable
{
   private int      miIngrediente ;
   private Estanco  estanco ;
   public  Thread   thr ;


   //-------------------------------------------------------------------

   public Fumador( int p_miIngrediente , Estanco p_estanco)
   {

      miIngrediente = p_miIngrediente ;
      estanco       = p_estanco ;
      thr         = new Thread(this,"Fumador "+miIngrediente);

    }

   //-------------------------------------------------------------------

   public void run ()
   {


      while (true)
      {
     System.out.println("Estoy despierto");
		 estanco.obtenerIngredientes(miIngrediente);
     System.out.println("me han despertado");
		 aux.dormir_max( 3000 );
	   System.out.println("fumador "+miIngrediente+"termina de fumar");
      }
   }

}

//**********************************************************************

class Estanquero implements Runnable
{
   private Estanco estanco ;
   public  Thread  thr ;

   //-------------------------------------------------------------------

   Estanquero( Estanco p_estanco )
   {

       estanco = p_estanco ;
       thr   = new Thread(this,"estanquero");

   }

   //-------------------------------------------------------------------

   public void run ()
   {
      int fumador_siguiente ;

      while (true)
      {
         fumador_siguiente = (int) (Math.random () * 3.0);

         estanco.ponerIngredientes( fumador_siguiente);
         estanco.esperarRecogidaIngredientes();

	 aux.dormir_max( 100 );
      }
   }
}

//**********************************************************************


class fumadores_D1_B
{
	public static void main( String[] arg )
	{
		System.out.println("Fumadores:");

		Estanco    estanco    = new Estanco() ;
		Estanquero estanquero = new Estanquero(estanco) ;
		Fumador[]  fumador    = new Fumador[3] ;


		fumador[0] = new Fumador(0, estanco);
		fumador[1] = new Fumador(1, estanco);
		fumador[2] = new Fumador(2, estanco);

		estanquero.thr.start();
		for( int i = 0 ; i < 3 ; i++ )
			fumador[i].thr.start();





	}
}
