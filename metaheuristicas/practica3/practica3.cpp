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


#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

#include <string.h>
#include "./clases/CPH.h"

using namespace std;
using namespace clases;

// Incluir los namespaces que vayais a utilizar


// Función de procesamiento. Es una plantilla, además de los argumentos habituales
// recibe como argumento el tipo de la clase de problema a procesar, y el tipo de
// las soluciones correspondientes

template <class PROBLEMA,typename datosSolucion>
int procesarDatos(char *nombre, char * tipo, char * problema, string rvalue) {
	int error=EXIT_SUCCESS;
	PROBLEMA *p = new PROBLEMA();
	if((error=p->leerInstancia(nombre))!=1){
		cout << "Error al leer la instancia del problema" << endl;
		return error;
	}

	//p->imprimirInstancia();

	datosSolucion *x = new datosSolucion();
	x = p->generarSolucionRandom();
	datosSolucion * solucion = new datosSolucion();

	float valorObtenido;

	if(strcmp(tipo,"AGe")==0)
	{
		solucion=p->aplicarAlgoritmoGenetico(50, 0.9, 0.15, 3500, true, rvalue);
		valorObtenido = p->evaluarSolucion(solucion);
	}
	else
	{
		solucion=p->aplicarAlgoritmoGenetico(50, 0.9, 0.15, 3500, false, rvalue);
		valorObtenido = p->evaluarSolucion(solucion);
	}

	cout << "Función objetivo final: " << valorObtenido << endl;
	p->liberarMemoriaSolucion(x);

	delete p;
	return error;
}


int main(int argc, char **argv) {

	// Procesamiento de la línea de comandos

	int c;
	bool fflag, pflag, sflag;
	char * pvalue, * fvalue, * avalue;
	string rvalue;
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
				avalue = optarg;
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

	if(pflag && strcmp(pvalue,"CPH")==0)
	{
		cout << "Problema CPH. Instancia " << fvalue << "..." << endl;
		cout << "===============================================" << endl;
		string str(fvalue);
		string semillastr; //voy a convertir la semilla a str

		ostringstream convert;
		convert << semilla;

		semillastr = convert.str();

		rvalue = str +"_resultados.txt";
		rvalue.erase(0,15);
		rvalue = "./resultados/" + rvalue + "_" + semillastr;

		cout << rvalue << endl;
			if(procesarDatos<CPH,struct datosSolucionCPH>(fvalue, avalue, pvalue, rvalue) != 1)
				return EXIT_FAILURE;
			return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}

