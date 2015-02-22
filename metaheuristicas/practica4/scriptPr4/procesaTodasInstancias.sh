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

		nombreFichero=$(basename $f)

		mejorValor=$(cat best_values_TSP.txt | grep $nombreFichero | cut -d ' ' -f 2)

		tiempoInicio=$(date +%s.%N)
		salida=$(./acotsp --tsplibfile $f --acs --rho 0.5 --q0 0.0 --time 5 --optimum $mejorValor --ants 100 --tries 3 --localsearch 0 | grep "found at time")
		valor=$(echo $salida | cut -d ' ' -f 4 | tr -d ',')
		tiempo=$(echo "$(date +%s.%N)-$tiempoInicio" | bc)
		echo "$nombreFichero,$valor,$tiempo,$mejorValor" >> $temporal
		
	fi
done

echo "Fichero,ValorACS,TiempoACS,MejorValor" > $2
sort -t ',' -k 1,1 $temporal >> $2
rm $temporal
