#!/bin/csh
@ inicio = 100000
@ fin = 20000000
@ incremento = 500000
set ejecutable = calculoSecuencialVersionSimple
set salida = tiemposCalculoSecuencial.dat

@ i = $inicio
echo > $salida
while ( $i <= $fin )
  echo Ejecución tam = $i
  echo `./{$ejecutable} $i ` >> $salida
  @ i += $incremento
end
	