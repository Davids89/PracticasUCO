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
		if [[ $f == *MMDP* ]]
		then
			tipo="MMDP"
		#elif [[ $f == *CPH* ]]
		#then
		#	tipo="CPH"
		#elif [[ $f == *CWP* ]]
		#then
		#	tipo="CWP"
		elif [[ $f == *TSP* ]]
		then
			tipo="TSP"
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
				salida=$(./practica2 -p $tipo -f "$f" -s $s -a BLb | grep "Función objetivo final:")
				valor=$(echo $salida | cut -d ' ' -f 4)
				tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
				echo -n "$nombreFichero,$tipo,$valor,$tiempo," >> $temporal

				tiempoInicio=$(date +%s.%N)
				salida=$(./practica2 -p $tipo -f "$f" -s $s -a BLf | grep "Función objetivo final:")
				valor=$(echo $salida | cut -d ' ' -f 4)
				tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
				echo -n "$valor,$tiempo," >> $temporal

				tiempoInicio=$(date +%s.%N)
				salida=$(./practica2 -p $tipo -f "$f" -s $s -a ES | grep "Función objetivo final:")
				valor=$(echo $salida | cut -d ' ' -f 4)
				tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
				echo "$valor,$tiempo,$mejorValor" >> $temporal
			done
		fi
		
	fi
done

echo "Fichero,Tipo,ValorBLb,TiempoBLb,ValorBLf,TiempoBLf,ValorES,TiempoES,MejorValor" > $2
sort -t ',' -k 2,2 -k 1,1 $temporal >> $2
rm $temporal
