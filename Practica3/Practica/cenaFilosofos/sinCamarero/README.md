#Cena de los filósofos (sin camarero)

####Aspectos más destacados de la solución al problema, situación de interbloqueo y solución.

Lo más destacable de la solución propuesta sería el desarrollo de los bucles del proceso *tenedor* y *filosofo* y el uso de tag para la comunicación entre los procesos que ejecutan uno y otro método.

######Tenedor

El proceso tenedor empieza escuchando mensajes de cualquier proceso (MPI_ANY_SOURCE) que los envíe dentro del comunicador común (MPI_COMM_WORLD) pero que use como tag el id del propio proceso tenedor. Así este se asegura que los mensajes que recibe son los que van dirigidos exclusivamente a él.

Cuando recibe el mensaje usa la variable de tipo MPI_Status que usa en MPI_Recv para conocer el origen de este. Como sabemos la distribución de los comensales y de los cubiertos en la mesa podemos determinar a partir del *id*  del proceso que envió el mensaje si se trata del comensal de la derecha o de la izquierda.
Una vez sabido esto enviamos la respuesta al filósofo simulando así el uso del tenedor quedandonos después a la espera de que nos devuelva el control del mismo. Esto sucede igual independientemente de que sea el comensal de la izquierda o de la derecha.

Recuperado el control se repite el proceso empezando de nuevo por la escucha de los mensajes de los filósofos adyacentes.


######Filosofo

Todos los procesos filósofos empiezan intentando coger el tenedor de su izquierda (cuestiones de protocolo), excepto aquel que haya sido designado con el *id* 0, que lo hará al revés, con lo que se evita el interbloqueo en el posible caso de que todos realicen al mismo tiempo la petición , les sea asignado el tenedor izquierdo y por tanto se produzca el interbloqueo cuando ninguno pueda obtener el derecho y por tanto no pueda comer.

Dejando esto a un lado, el proceso realiza la petición de ambos tenedores y espera a que el proceso tenedor le responda con la simulación del "permiso de uso " de este.

Cuando un filósofo tiene los dos tenedores se dispone a comer empleando un tiempo aleatorio. Tras esto libera los tenedores y se pone a pensar un rato, durante un tiempo también aleatório.

Ambos procesos pueden sincronizarse gracias al envío de mensajes de tipo seguro y bloqueantes Ssend que bloquean al proceso que los ejecuta hasta que el mensaje ha sido recibido.

####Código fuente.

El código fuente lo tenemos en [**filosofos.cpp**](filosofos.cpp) y se encuentra lo suficientemente comentado como para entender su funcionamiento de un vistazo.

####Salida parcial del programa.

En el siguiente fragmento de la salida de la ejecución se puede comprobar que los cinco filósofos comen cuando consiguen el uso de ambos tenedores adyacentes y como los van liberando para que los puedan usar el resto de filósofos.

~~~bash
Ubuntu> mpicxx -o filosofos filosofos.cpp ; mpirun -np 10 filosofosFilosofo 0 se sienta a la mesa
Filosofo 3 se sienta a la mesa
Filosofo 3 solicita tenedor izq, proceso y tag: 7
Filosofo 0 solicita tenedor izq, proceso y tag: 9
Tenedor n. 4 con filosofos 4 a la derecha y 0 a la izquierda.
Ten. 4 recibe petic. de filósofo 0 proces: 0
Ten 4 Envío a filosofo: 0 num proc. 0 con tag 9
Filosofo 0 solicita tenedor der, proceso: 1
Tenedor n. 1 con filosofos 1 a la derecha y 2 a la izquierda.
Tenedor n. 3 con filosofos 3 a la derecha y 4 a la izquierda.
Tenedor n. 0 con filosofos 0 a la derecha y 1 a la izquierda.
Tenedor n. 2 con filosofos 2 a la derecha y 3 a la izquierda.
Filosofo 2 se sienta a la mesa
Ten. 3 recibe petic. de filósofo 3 proces: 6
Filosofo 3 solicita tenedor der, proceso: 5
Filosofo 4 se sienta a la mesa
Ten 3 Envío a filosofo: 3 num proc. 6 con tag 7
Ten. 0 recibe petic. de filósofo 0 proces: 0
FIlosofo: 0Esperando respuesta de uso del tenedor de la izquierda, proc 9
RECIBIDO TENEDOR de IZQUIERDA
FIlosofo: 0Esperando respuesta de uso del tenedor de la derecha, proc 9
Ten 0 Envío a filosofo: 0 num proc. 0 con tag 1
RECIBIDO TENEDOR de DERECHA
Ten 0 queda esperando liberación de  0
Ten. 2 recibe petic. de filósofo 3 proces: 6
FIlosofo: 3Esperando respuesta de uso del tenedor de la izquierda, proc 7
RECIBIDO TENEDOR de IZQUIERDA
FIlosofo: 3Esperando respuesta de uso del tenedor de la derecha, proc 7
Filosofo 0 ## COMIENDO ##
Ten 3 queda esperando liberación de  3
Ten 2 Envío a filosofo: 3 num proc. 6 con tag 5
RECIBIDO TENEDOR de DERECHA
Filosofo 3 ## COMIENDO ##
Filosofo 2 solicita tenedor izq, proceso y tag: 5
Filosofo 4 solicita tenedor izq, proceso y tag: 9
Filosofo 1 se sienta a la mesa
Filosofo 1 solicita tenedor izq, proceso y tag: 3
Ten. 1 recibe petic. de filósofo 1 proces: 2
Ten 1 Envío a filosofo: 1 num proc. 2 con tag 3
Filosofo 1 solicita tenedor der, proceso: 1
Filosofo 0 suelta tenedor izq ...9
Filosofo 6 suelta tenedor izq ...7
Ten. 9 recibe liberac. de 0
Filosofo 0 suelta tenedor der ...1
Filosofo 0 PENSANDO
Ten. 4 recibe petic. de filósofo 4 proces: 8
Ten 4 Envío a filosofo: 4 num proc. 8 con tag 9
Filosofo 4 solicita tenedor der, proceso: 7
FIlosofo: 4Esperando respuesta de uso del tenedor de la izquierda, proc 9
Ten. 1 recibe liberac. de 0
Ten. 0 recibe petic. de filósofo 1 proces: 2
Ten 0 Envío a filosofo: 1 num proc. 2 con tag 1
Ten. 7 recibe liberac. de 3
Ten. 3 recibe petic. de filósofo 4 proces: 8
Ten 3 Envío a filosofo: 4 num proc. 8 con tag 7
Ten 4 queda esperando liberación de  4
RECIBIDO TENEDOR de IZQUIERDA
FIlosofo: 4Esperando respuesta de uso del tenedor de la derecha, proc 9
RECIBIDO TENEDOR de DERECHA
Filosofo 4 ## COMIENDO ##
Filosofo 6 suelta tenedor der ...5
Filosofo 6 PENSANDO
Ten. 5 recibe liberac. de 3
Ten. 2 recibe petic. de filósofo 2 proces: 4
Ten 2 Envío a filosofo: 2 num proc. 4 con tag 5
Filosofo 2 solicita tenedor der, proceso: 3
FIlosofo: 1Esperando respuesta de uso del tenedor de la izquierda, proc 3
RECIBIDO TENEDOR de IZQUIERDA
Ten 1 queda esperando liberación de  1
FIlosofo: 1Esperando respuesta de uso del tenedor de la derecha, proc 3
RECIBIDO TENEDOR de DERECHA
Filosofo 1 ## COMIENDO ##
Filosofo 0 solicita tenedor izq, proceso y tag: 9
Filosofo 3 solicita tenedor izq, proceso y tag: 7
Filosofo 8 suelta tenedor izq ...9
Ten. 9 recibe liberac. de 4
Filosofo 8 suelta tenedor der ...7
Ten. 7 recibe liberac. de 4
Filosofo 2 suelta tenedor izq ...3
Filosofo 8 PENSANDO
Ten. 3 recibe liberac. de 1
Filosofo 2 suelta tenedor der ...1
Ten. 1 recibe liberac. de 1
Filosofo 2 PENSANDO
Filosofo 3 solicita tenedor der, proceso: 5
Ten. 3 recibe petic. de filósofo 3 proces: 6
Ten 3 Envío a filosofo: 3 num proc. 6 con tag 7
Ten. 1 recibe petic. de filósofo 2 proces: 4
Ten 1 Envío a filosofo: 2 num proc. 4 con tag 3
Ten. 4 recibe petic. de filósofo 0 proces: 0
Ten 4 Envío a filosofo: 0 num proc. 0 con tag 9
FIlosofo: 2Esperando respuesta de uso del tenedor de la izquierda, proc 5
RECIBIDO TENEDOR de IZQUIERDA
Ten 2 queda esperando liberación de  2
FIlosofo: 2Esperando respuesta de uso del tenedor de la derecha, proc 5
RECIBIDO TENEDOR de DERECHA
Filosofo 2 ## COMIENDO ##
Filosofo 0 solicita tenedor der, proceso: 1
Ten. 0 recibe petic. de filósofo 0 proces: 0
FIlosofo: 0Esperando respuesta de uso del tenedor de la izquierda, proc 9
Ten 0 Envío a filosofo: 0 num proc. 0 con tag 1
RECIBIDO TENEDOR de IZQUIERDA
FIlosofo: 0Esperando respuesta de uso del tenedor de la derecha, proc 9
RECIBIDO TENEDOR de DERECHA
Ten 0 queda esperando liberación de  0
Filosofo 0 ## COMIENDO ##
Filosofo 4 solicita tenedor izq, proceso y tag: 9
Filosofo 1 solicita tenedor izq, proceso y tag: 3
Filosofo 0 suelta tenedor izq ...9
Ten. 9 recibe liberac. de 0
Ten. 4 recibe petic. de filósofo 4 proces: 8
Ten 4 Envío a filosofo: 4 num proc. 8 con tag 9
Filosofo 0 suelta tenedor der ...1
Ten. 1 recibe liberac. de 0
Filosofo 0 PENSANDO
Filosofo 4 solicita tenedor der, proceso: 7
~~~
