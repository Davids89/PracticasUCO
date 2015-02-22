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
#include "clases/MMDP.h"
#include "clases/TSP.h"
#include <cmath>
#include <time.h>

using namespace std;
using namespace clases;

// Incluir los namespaces que vayais a utilizar


// Función de procesamiento. Es una plantilla, además de los argumentos habituales
// recibe como argumento el tipo de la clase de problema a procesar, y el tipo de
// las soluciones correspondientes

template <class PROBLEMA,typename datosSolucion>
int procesarDatos(char *nombre, char * tipo, char * modo) {
	int error=EXIT_SUCCESS;
	//cerr << "Hola" << endl;
	PROBLEMA *p = new PROBLEMA();//hay que hacer un constructor nuevo
	if((error=p->leerInstancia(nombre))!=1){
		cout << "Error al leer la instancia del problema" << endl;
		return error;
	}

	cout << "Se va a imprimir la instancia" << endl;
	p->imprimirInstancia();
	/*float maximo = 0;
	float minimo = 0;*/

	datosSolucion *x = new datosSolucion;
	x = p->generarSolucionRandom();
	datosSolucion * solucion1 = new datosSolucion;

	//aqui hay que evaluar que tipo de problema es y que tipo de algoritmo se usa
	if(strcmp(tipo,"MMDP")==0) //estamos en MMDP
	{
		if(strcmp(modo, "BLb") == 0)//estamos en MMDP con BLB
		{
			solucion1 = p->aplicarBusquedaLocal(x, false, 50);
		}
		else if(strcmp(modo, "BLf") == 0)//estamos en MMDP con BLF
		{
			solucion1 = p->aplicarBusquedaLocal(x, true, 50);
		}
		else //estamos en MMDP con ES
		{
			float tFinal = pow(10, -4);
			solucion1 = p->aplicarEnfriamientoSimulado(x, tFinal, 10, 50);
		}
	}
	else //estamos en TSP
	{
		if(strcmp(modo, "BLb") == 0) //TSP con BLB
		{
			cout << "TSP con BLB" << endl;
			solucion1 = p->aplicarBusquedaLocal(x, false, 1000);
		}
		else if(strcmp(modo, "BLf") == 0) //TSP con BLF
		{
			cout << "TSP con BLF" << endl;
			solucion1 = p->aplicarBusquedaLocal(x, true, 1000);
		}
		else //TSP con ES
		{
			cout << "TSP con ES" << endl;
			float tFinal = pow(10, -4);
			solucion1 = p->aplicarEnfriamientoSimulado(x, tFinal, 10, 1000);
		}
	}

	float valorFinal = p->evaluarSolucion(solucion1);
	cout << "Función objetivo final: " << valorFinal << endl;

	/*p->liberarMemoriaSolucion(solucion1);
	delete p;*/
	return error;
}


int main(int argc, char **argv) {

	// Procesamiento de la línea de comandos

	int c;
	bool fflag, pflag, sflag;
	char * pvalue, * fvalue, * tipo;
	int semilla;

	while( (c = getopt(argc, argv, "f:p:s:a:")) != -1)
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
			case 'a':
				tipo = optarg;
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
		if(procesarDatos<MMDP,struct datosSolucionMMDP>(fvalue, pvalue, tipo) != 1)
			return EXIT_FAILURE;
		return EXIT_SUCCESS;
	}

	else if(pflag && strcmp(pvalue,"TSP")==0)
	{
		cout << "Problema TSP. Instancia" << fvalue << "..." << endl;
		cout << "===============================================" << endl;
			if(procesarDatos<TSP,struct datosSolucionTSP>(fvalue, pvalue, tipo) != 1)
				return EXIT_FAILURE;
			return EXIT_SUCCESS;
	}
	// Lo mismo para TSP, CWP y CPH (else if...)
	// Por defecto, el MMDP
	else{
		cout<< "Problema MMDP. Instancia " << fvalue << "..."<< endl;
		if(procesarDatos<MMDP,struct datosSolucionMMDP>(fvalue, pvalue, tipo) != 1)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

