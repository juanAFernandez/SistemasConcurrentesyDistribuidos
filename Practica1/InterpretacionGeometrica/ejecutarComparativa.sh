#!/bin/bash
echo ""
echo "--------------------------------------------------------------------"
echo "  Comparación de ejecución Secuencian y Concurrente para dos CPU    "
echo "  Autor: Juan Antonio Fernández Sánchez						      "
echo "  Publicado en: github.com/juanAFernandez		  					  "
echo "--------------------------------------------------------------------"
echo ""

echo "Compilación de los fuentes..."
g++ calculoSecuencialVersionSimple.cpp -o calculoSecuencialVersionSimple -lrt
g++ calculoConcurrente.cpp -o calculoConcurrente -lrt
sleep 2

echo ""
echo "Cálculo de los tiempos de ejecución para el código SECUENCIAL:"
echo ""
sleep 2
T1="$(date +%s)"
./ejecucionesSecuencial.csh
T1="$(($(date +%s)-T1))"

echo ""
echo "Cálculo de los tiempos de ejecución para el código CONCURRENTE:"
echo ""
sleep 2
T2="$(date +%s)"
./ejecucionesConcurrente.csh
T2="$(($(date +%s)-T2))"

echo ""
echo "Creación de gráficas en fichero graficaComparativa.jpeg"
echo ""

sleep 2
gnuplot creacionGrafica.gp

echo ""
echo "Tiempo de ejecución secuencial total: ${T1} segundos."
echo "Tiempo de ejecución concurrente total:  ${T2} segundos."
echo ""

echo "Mostrando gráfica y guardando en el lugar de la ejecución"
echo "***"


