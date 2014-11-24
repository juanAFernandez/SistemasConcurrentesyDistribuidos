Los monitores tipo Hoare (1974) son un mecanismo de alto nivel que permite definir objetos abstractos compartidos (una colección de datos y procedimientos asociados que se comparten por varios procesos).

El usuario sólo puede acceder al recurso mediante un conjunto de operaciones. El usuario ignora las variables que representan el recurso y la implementación de las operaciones asociadas.

El recurso compartido se percibe como un objeto al que se accede concurrentemente.



Propuesta de Hoare

– Un proceso suspendido debido a ejecución de signal, entrará en una cola de suspendidos
en Signal. Cada proceso antes de salir del monitor y liberar la E.M., comprueba si hay
procesos en esta cola, si los hay, heredarán la E.M. Los procesos suspendidos al ejecutar
una operación signal tienen prioridad sobre los que intentan entrar.

Comunicación monitor-mundo exterior: Cuando un proceso necesita operar sobre un recurso compartido controlado por un monitor deber ́ realizar una llamada a uno de los procedimientos exportados por el monitor usando los par ́metros reales apropiados.

Cuando un proceso est ́ ejecutando un procedimiento exportado del monitor decimos que est ́ dentro del monitor.
Exclusi ́n mutua: Si un proceso est ́ ejecutando un procedimiento del monitor (est ́ dentro del mismo), ning ́n otro proceso podr ́ entrar al monitor (a cualquier procedimiento del mismo) hasta que
el proceso que est ́ dentro del monitor termine la ejecuci ́n del procedimiento que ha invocado.

Esta pol ́ıtica de acceso asegura que las variables permanentes nunca son accedidas concurrentemente.
El acceso exclusivo entre los procedimientos del monitor debe estar garantizado en la implementación de los monitores.




wait
Estoy esperando a que alguna condición ocurra. Bloquea al proceso llamador

signal
Estoy señalando que una condición ocurre. Reactiva a un proceso esperando dicha condición (si no hay ninguna no hace nada).
