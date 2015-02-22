#!/bin/bash

cat << _end_ | gnuplot #para empezar a ejecutar
set terminal postscript eps color #ejecutar en color
set term png
set output "calculoDeterminanteRec.png" #nombre del grafico
set key right bottom #
set xlabel "Etiqueta eje X (Elementos)" #etiqueta eje x
set ylabel "Etiqueta Eje Y (Tiempo)" #etiqueta eje y
plot 'prueba.txt' using 1:3 t "Recursivo", 'prueba.txt' using 1:4 t "EstimadoRecursivo" w l #la leyenda es la que aparece diciendo que tales puntos o curvas es de tal tipo
_end_
