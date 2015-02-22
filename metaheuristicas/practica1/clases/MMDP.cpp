/*
 * MMDP.cpp
 *
 *  Created on: 21/02/204
 *      Author: pedroa
 */

#include "MMDP.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

// INCLUDES NECESARIOS

using namespace std;

namespace clases {

MMDP::MMDP() {
	data.n = 0;
	data.m = 0;
}

MMDP::~MMDP() {
	liberarMemoria();
}

int MMDP::reservarMemoria() {

	try
	{
		data.distancia = new float * [data.n];

		for (int i=0; i<data.n; i++)
		{
			data.distancia[i] = new float [data.n];
		}

		for(int i=0; i<data.n; i++)
		{
			for(int j=0; j<data.n; j++)
			{
				data.distancia[i][j]=-1;
			}
		}

	}catch(int e)
	{
		cerr << "Excepcion encontrada al reservar memoria" << endl;
		return(-1);
		//ha fallado y se devuelve un -1
	}

	return 1; //ha ido bien y se devuelve un 1
}

int MMDP::reservarMemoriaSolucion(struct datosSolucionMMDP * solucion)
{
	solucion->vectorSolucion = new bool[data.n];
	for(int i; i<data.n; i++)
	{
		solucion->vectorSolucion[i]=false;
	}
	//return solucion;
	return 1;
}

void MMDP::liberarMemoria() {

	for(int i; i<data.n; i++)
	{
		delete [] data.distancia[i];
	}

	delete [] data.distancia;
}

void MMDP::liberarMemoriaSolucion(struct datosSolucionMMDP *sol){

	delete[] sol->vectorSolucion;

}

int MMDP::leerInstancia(char *archivo) {

	//abrir el archivo
	ifstream fichero;
	fichero.open(archivo);

	//cogemos la primera linea para sacar el numero m y n

	char cadena[128];
	char * numero;

	fichero.getline(cadena, 128);
	numero=strtok(cadena, " ");
	data.n = atoi(numero);

	numero=strtok(NULL, " ");
	data.m = atoi(numero);

	//reservar memoria

	int memoria;
	memoria = reservarMemoria();

	if(memoria == -1)
	{
		exit(-1);
	}

	//terminar de leer instancia

	float distancia;
	int i,j;

	while(!fichero.eof())
	{
		fichero >> i >> j >> distancia;
		data.distancia[i][j] = data.distancia[j][i] = distancia;
	}

	fichero.close();

	return 1;

}


float MMDP::evaluarSolucion(struct datosSolucionMMDP *sol) {

	float minimo = 9999;

			for(int i = 0; i < data.n; i++)
			{
				if(sol->vectorSolucion[i] == true)
				{
					for(int j = 0; j < data.n; j++)
					{
						if(sol->vectorSolucion[j] == true and i != j)
						{
							if(data.distancia[i][j] < minimo)
							{
								minimo = data.distancia[i][j];
							}
						}
					}
				}
			}
			return minimo;
}

struct datosSolucionMMDP * MMDP::generarSolucionRandom() {

	//Reserva de memoria de los vectores de numeros
			int * numerosPorElegir = new int [data.n];
			int * numerosElegidos = new int [data.n];

			//Se inicializa la lista de elecciones posibles
			for(int i = 0; i < data.n; i++)
			{
				numerosPorElegir[i] = i;
			}

			for(int i = 0; i < data.n; i++)
			{
				int numeroElegido = rand()%(data.n-i);
				numerosElegidos[i] = numerosPorElegir[numeroElegido];//Recordar el numero elegido
				numerosPorElegir[numeroElegido] = numerosPorElegir[data.n-i-1];//Se ponen en numeros por elegir, el ultimo que era valido, asi son todos validos hasta n-i-1
			}

			//Se libera la memoria del vector que ya no se usara
			delete [] numerosPorElegir;

			//Se reserva memoria del vector solucion
			struct datosSolucionMMDP * solucion = new struct datosSolucionMMDP;
			reservarMemoriaSolucion(solucion);

			for(int i = 0; i < data.m; i++)
			{
				solucion->vectorSolucion[numerosElegidos[i]] = true;
			}

			return solucion;
}

void MMDP::imprimirSolucion(struct datosSolucionMMDP *sol) {

	cout << "**************************************" << endl;
	cout << "Vector solucion" << endl;

	for(int i=0; i<data.n; i++)
	{
		if(sol->vectorSolucion[i] == true)
			cout << "1 ";
		else
			cout << "0 ";
		//cout << data.distancia[i] << " ";
	}
	cout << "\n**************************************" << endl;
	cout << endl;
}

void MMDP::imprimirInstancia() {

	cout << "========================================================================================" << endl;
	cout << "Dimensiones:" << endl;
	cout << "m=" << data.m << " n=" << data.n << endl;
	cout << "========================================================================================" << endl;
	cout << "Matriz de conectividad" << endl;

	for(int i=0; i<data.n; i++)
	{
		for(int j=0; j<data.n; j++)
		{
			cout << "\t" << data.distancia[i][j];
		}
		cout << endl;
	}

	cout << "========================================================================================" << endl;
}
}
