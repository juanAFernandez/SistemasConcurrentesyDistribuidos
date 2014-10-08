set terminal jpeg
set output "graficaComparativa.jpeg"
plot "tiemposCalculoConcurrente.dat","tiemposCalculoSecuencial.dat"
set terminal wxt size 1000,400 enhanced font 'Verdana,10' persist
plot "tiemposCalculoConcurrente.dat","tiemposCalculoSecuencial.dat"
