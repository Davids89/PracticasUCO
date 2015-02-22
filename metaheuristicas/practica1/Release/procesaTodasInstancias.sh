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
		tiempoInicio=$(date +%s.%N)
		if [[ $f == *MMDP* ]]
		then
			tipo="MMDP"
		elif [[ $f == *CPH* ]]
		then
			tipo="CPH"
		elif [[ $f == *CWP* ]]
		then
			tipo="CWP"
		elif [[ $f == *TSP* ]]
		then
			tipo="TSP"
		else
			echo "Fichero $f no reconocido"
		fi
		if [ ! -z "$tipo" ]
		then
			salida=$(./practica1 -p $tipo -f "$f" -s 80 | grep "extremos")
			maximo=$(echo $salida | sed 's/.*máximo=\([^ ]*\).*/\1/')
			minimo=$(echo $salida | sed 's/.*mínimo=\([^ ]*\).*/\1/')
			tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
			echo "\"`basename $f`\",$tipo,$minimo,$maximo,$tiempo" >> $temporal
		fi
		
	fi
	
done

echo "Fichero,Tipo,Mínimo,Máximo,Tiempo" > $2
sort -t ',' -k 2,2 -k 1,1 $temporal >> $2
rm $temporal
