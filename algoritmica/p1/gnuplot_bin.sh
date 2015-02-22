#!/bin/bash

cat << _end_ | gnuplot #para empezar a ejecutar
set terminal postscript eps color #ejecutar en color
set term png
set output "graficoBinario.png" #nombre del grafico
set key right bottom #
set xlabel "Etiqueta eje X (Repeticiones)" #etiqueta eje x
set ylabel "Etiqueta Eje Y (Tiempo)" #etiqueta eje y
plot 'fichero.txt' using 1:4 t "BBinaria", 'fichero.txt' using 1:5 t "BEBinaria" w l #la leyenda es la que aparece diciendo que tales puntos o curvas es de tal tipo
_end_
