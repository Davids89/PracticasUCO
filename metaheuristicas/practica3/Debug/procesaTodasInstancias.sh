#!/bin/bash
if [ $# != 2 ]
then
	echo "Debes llamar a este script de la siguiente forma:"
	echo "$0 nombre-de-la-carpeta-con-todas-las-instancias fichero.csv"
	exit 0
fi

temporal=$(mktemp)
OLDIFS=$IFS
IFS=$'\n'

archivos=($(find "$1" -name "*"))

IFS=$OLDIFS

for f in "${archivos[@]}"
do
	if [ -f "$f" ]
	then
		echo "Procesando el fichero $f..."
		if [[ $f == *CPH* ]]
		then
			tipo="CPH"
		else
			echo "Fichero $f no reconocido"
		fi
		if [ ! -z "$tipo" ]
		then
			for s in 10 20 30
			do
				nombreFichero=$(basename $f)

				mejorValor=$(cat best_values_$tipo.txt | grep $nombreFichero | cut -d ' ' -f 2)

				tiempoInicio=$(date +%s.%N)
				salida=$(./practica3 -p $tipo -f "$f" -s $s -a AGg | grep "Función objetivo final:")
				valor=$(echo $salida | cut -d ' ' -f 4)
				echo "Algoritmo AGg:" $valor
				tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
				echo -n "$nombreFichero,$tipo,$valor,$tiempo," >> $temporal

				tiempoInicio=$(date +%s.%N)
				salida=$(./practica3 -p $tipo -f "$f" -s $s -a AGe | grep "Función objetivo final:")
				valor=$(echo $salida | cut -d ' ' -f 4)
				echo "Algoritmo AGe:" $valor
				tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
				echo "$valor,$tiempo,$mejorValor" >> $temporal
			done
		fi
		
	fi
done

echo "Fichero,Tipo,ValorAGg,TiempoAGg,ValorAGe,TiempoAGe,MejorValor" > $2
sort -t ',' -k 2,2 -k 1,1 $temporal >> $2
rm $temporal
