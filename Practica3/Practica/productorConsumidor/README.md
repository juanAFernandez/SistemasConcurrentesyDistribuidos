#Productor-Consumidor con Open-MPI


####Idea:

Se parte de los código *prodcons.cpp* y *prodcons2.cpp* para entender el funcionamiento básico de un productor-consumidor implementado con Open-MPI. A partir de la segunda versión se pide una extensión para un número de procesos más elevado.

####Desarrollo:

Extender el programa MPI *prodcons2.cpp* que implementa el productor-consumidor con buffer acotado para que el proceso buffer dé servicio a 5 productores y 4 consumidores. Para elo, se lanzarán 10 procesos y asumiremos que los procesos 0...4 son productores, el proceso Buffer es el proceso 5 y el resto de procesos en el comunicador universal (6...9) son consumidores.

####Una posible solución:

La resolución del problema puede abordarse de muchas formas, en este caso se ha realizado de una en concreto que no tiene por qué ser la mejor ni la única.




Nota sobre los mensajes:

Se ha decidido que sólo sea en el método **buffer** donde se impriman mensajes por terminal ya que la concurrencia de los 10 procesos puede hacer entremezclase a estos en el buffer de impresión por terminal. Sin embargo si sólo se hacen impresiones en **buffer** todos serán completamente síncronos a la ejecución del código y podrá hacerse un mejor seguimiento esta.
