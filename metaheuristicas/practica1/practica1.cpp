//============================================================================
// Name        : practica1.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
// Description : Práctica 1 de Metaheurísticas
//============================================================================

// Realizar los import necesarios

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "./clases/MMDP.h"
#include "./clases/TSP.h"
#include "./clases/CWP.h"
#include "./clases/CPH.h"

using namespace std;
using namespace clases;

// Incluir los namespaces que vayais a utilizar


// Función de procesamiento. Es una plantilla, además de los argumentos habituales
// recibe como argumento el tipo de la clase de problema a procesar, y el tipo de
// las soluciones correspondientes

template <class PROBLEMA,typename datosSolucion>
int procesarDatos(char *nombre) {
	int error=EXIT_SUCCESS;
	PROBLEMA *p = new PROBLEMA();
	if((error=p->leerInstancia(nombre))!=1){
		cout << "Error al leer la instancia del problema" << endl;
		return error;
	}

	p->imprimirInstancia();
	float maximo = 0;
	float minimo = 0;
	for(int i=0; i<1000; i++){
		datosSolucion *x = p->generarSolucionRandom();
		cout << "-----------------------------------" << endl;
		cout << "Solución generada (iteracion " << i << "): " << endl;
		p->imprimirSolucion(x);
		float valorObtenido = p->evaluarSolucion(x);
		if(i==0){
			maximo = minimo = valorObtenido;
		}
		else{
			if(valorObtenido>maximo)
				maximo = valorObtenido;
			if(valorObtenido<minimo)
				minimo = valorObtenido;
		}
		cout << "Función objetivo: " << valorObtenido << endl;
		p->liberarMemoriaSolucion(x);
	}
	cout << endl << endl << "==> Valores extremos generados: máximo=" << maximo << " mínimo=" << minimo << endl;

	delete p;
	return error;
}


int main(int argc, char **argv) {

	// Procesamiento de la línea de comandos

	int c;
	bool fflag, pflag, sflag;
	char * pvalue, * fvalue;
	int semilla;

	while( (c = getopt(argc, argv, "f:p:s:")) != -1)
	{
		switch(c)
		{
			case 'f':
				fflag = true;
				fvalue = optarg;
				break;

			case 'p':
				pvalue = optarg;
				pflag = true;
				break;

			case 's':
				semilla = atoi(optarg);
				sflag = true;
				break;

			default:
				cout << "No has introducido parametros: se ajustaran por defecto" << endl;


		}
	}

	// Si no se ha especificado nombre de fichero
	if(!fflag){
		cout<< "Es necesario especificar el nombre del fichero a procesar" << endl;
		return -1;
	}

	// Establecer la semilla a lo que ha dicho el usuario
	if(sflag){
		srand(semilla);
	}
	// Establecer la semilla usando time(NULL)
	else{
		srand(time(NULL));
	}


	// Tipo de problema
	if(pflag && strcmp(pvalue,"MMDP") == 0){
		cout<< "Problema MMDP. Instancia " << fvalue << "..."<< endl;
		cout << "===============================================" << endl;
		if(procesarDatos<MMDP,struct datosSolucionMMDP>(fvalue) != 1)
			return EXIT_FAILURE;
		return EXIT_SUCCESS;
	}

	else if(pflag && strcmp(pvalue,"TSP")==0)
	{
		cout << "Problema TSP. Instancia" << fvalue << "..." << endl;
		cout << "===============================================" << endl;
			if(procesarDatos<TSP,struct datosSolucionTSP>(fvalue) != 1)
				return EXIT_FAILURE;
			return EXIT_SUCCESS;
	}

	else if(pflag && strcmp(pvalue,"CWP")==0)
	{
		cout << "Problema CWP. Instancia" << fvalue << "..." << endl;
		cout << "===============================================" << endl;
			if(procesarDatos<CWP,struct datosSolucionCWP>(fvalue) != 1)
				return EXIT_FAILURE;
			return EXIT_SUCCESS;
	}

	else if(pflag && strcmp(pvalue,"CPH")==0)
	{
		cout << "Problema CPH. Instancia" << fvalue << "..." << endl;
		cout << "===============================================" << endl;
			if(procesarDatos<CPH,struct datosSolucionCPH>(fvalue) != 1)
				return EXIT_FAILURE;
			return EXIT_SUCCESS;
	}
	// Lo mismo para TSP, CWP y CPH (else if...)
	// Por defecto, el MMDP
	else{
		cout<< "Problema MMDP. Instancia " << fvalue << "..."<< endl;
		if(procesarDatos<MMDP,struct datosSolucionMMDP>(fvalue) != 1)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

