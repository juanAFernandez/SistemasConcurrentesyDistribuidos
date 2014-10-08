#!/bin/csh
@ inicio = 100000
@ fin = 20000000
@ incremento = 500000
set ejecutable = calculoConcurrente
set salida = tiemposCalculoConcurrente.dat

@ i = $inicio
echo > $salida
while ( $i <= $fin )
  echo Ejecución tam = $i
  echo `./{$ejecutable} $i ` >> $salida
  @ i += $incremento
end
	