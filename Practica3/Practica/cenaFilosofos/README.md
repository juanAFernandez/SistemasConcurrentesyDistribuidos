#La cena de los filósofos

Problema clásico de concurrencia que se usa mucho para ejemplificar el interbloqueo, nosotros vamos a dar una aproximación con paso de mensajes con **OpenMPI**.

**Idea**:

Tenemos una mesa redonda con 5 filosofos, que hacen dos cosas, meditar y comer. Cada uno tiene un tenedor y en el centro de la mesa hay una fuente de comida, la comida es muy resbaladiza y necesitan comer con dos tenedores. Cada filósofo solo puede usar los tenedores que tiene a adyacentes. Si sus dos tenedores no estan disponibles el no come.

Se trata de resolver este problema de concurrencia, lo vamos a simular de una forma un poco peculiar, tratando a los tenedores como procesos también, teniendo un total de diez procesos.  Y la acción de coger el tenedor lo simularemos con paso de mensajes **síncronos** (se pueden bloquear ambos procesos) entre el filósofo y el tenedor.

Por simplicidad los filósofos serán los números pares mientras que los tenedores serán los impares.

El tenedor de la izquierda es su identificador +1 módulo 10 y el de la derecha su identificador -1 módulo 10.


if(rank ==0)
  cout << "El numero de procesos debe ser 10"
Esto es para que el mensaje sólo salga una vez.

Recuerda: **todos los programas ejecutan exactamente el mismo código** No estamos hablando de hebras, sino de procedimientos. No lanzamos 5 filósofos y 5 tenedores, lanzo diez procesos que ejecutan el mismo código.

Los mesajes es más recomendable ponerlo después del sent, o dos uno antes y otro después, el último nos asegura que ya tenemos el tenedor en nuestro poder.

Interbloqueo: esta solución sirve para ver bien el interbloqueo porque según se inician los filósofos puede ser que los cinco cojan a la vez un tenedor y queden esperando al otro tenedor, entonces quedarán bloqueados indefinidiamente, es un error grave.
Esto debemos solucionarlo de alguna manera (evitar que se produzca el interbloqueo), por ejemplo, haciendo que alguno cambie el orden de petición de los tenedores, así forzamos a que dos filósofos compitan por coger el mismo tenedor y sabemos que uno de ellos lo cogerá, por tanto uno siempre podrá empezar a comer y el interbloqueo ya será imposible de ocurrir. Esto sería tan sencilo como añadir un condicional al código.

 ##Cena de los filósofos con camarero

 Se añade un camarero, ahora antes de coger los tenedores los filósofos tienen que sentarse en la mesa y para esto le piden permiso al camarero (que será otro proceso más, y si lo recibe podran sentarse y si no quedarán bloqueados). Con este nuevo proceso se elimina la posibilidad de interbloqueo, aunque existe un paso previo a comer (que será igual al ejercicio anterior). Puesto que ya no es posible el interbloqueo no debemos de implementar el cambio de preferencia de tenedor de uno de los filósofos.
