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
#include <cmath>

// INCLUDES NECESARIOS

using namespace std;

namespace clases {

MMDP::MMDP() {
	data.n = 0;
	data.m = 0;
	tipo = NULL;
}

MMDP::~MMDP()
{
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

struct datosSolucionMMDP * MMDP::reservarMemoriaSolucion()
{
	struct datosSolucionMMDP * solucion = new struct datosSolucionMMDP;
	solucion->vectorSolucion = new bool[data.n];
	for(int i=0; i<data.n; i++)
	{
		solucion->vectorSolucion[i]=false;
	}
	//return solucion;
	return solucion;
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

struct datosSolucionMMDP * MMDP::generarVecino(struct datosSolucionMMDP * solucion, int indice)
{
	struct datosSolucionMMDP * auxiliar = new struct datosSolucionMMDP;
	auxiliar = reservarMemoriaSolucion();
	//auxiliar = solucion;
	for (int i=0; i<data.n;i++)
		auxiliar->vectorSolucion[i] = solucion->vectorSolucion[i];

	int actual = 0;

	for (int i=0; i<data.n;i++)
	{
		for(int j=i+1; j<data.n; j++)//asi no se repiten vecinos
		{
			if(solucion->vectorSolucion[i] != solucion->vectorSolucion[j])
			{
				if(actual == indice)
				{
					auxiliar = intercambio(auxiliar, i, j);
				}
				actual++;
			}
		}
	}
	return auxiliar;
}

struct datosSolucionMMDP * MMDP::aplicarBusquedaLocal(struct datosSolucionMMDP *sol,  bool firstImprovement, int iterRelaTamano)
{
	int tam = data.m * (data.n - data.m)-1;
	int indice = 0;
	int n_soluciones=0;
	float valorActual = 0.0, mejorObjetivo = 0.0;
	int numeroElegido = 0;

	struct datosSolucionMMDP * solucion = new struct datosSolucionMMDP;
	solucion = reservarMemoriaSolucion();

	struct datosSolucionMMDP * vecino = new struct datosSolucionMMDP;
	vecino = reservarMemoriaSolucion();

	for(int i=0; i<data.n; i++)
	{
		solucion->vectorSolucion[i] = sol->vectorSolucion[i];
	}

	if(firstImprovement == true) //BLF
	{
		int i = 0;
		do
		{
			int * numerosPorElegir = new int[tam];
			int * numerosElegidos = new int[tam];

			for(int j=0;j<tam;j++)
			{
				numerosPorElegir[j]=j;
			}

			for (int j=0;j<tam;j++)
			{
				numeroElegido =rand()%(tam-j);

				numerosElegidos[j]=numerosPorElegir[numeroElegido];

				numerosPorElegir[numeroElegido]=numerosPorElegir[tam-j-1];

			}

			delete[] numerosPorElegir;

			valorActual = evaluarSolucion(solucion);

			do
			{
				indice = numerosElegidos[i];
				vecino = generarVecino(solucion, indice);

				n_soluciones++;
				i++;

			}while(evaluarSolucion(vecino) < valorActual);

			i=0;

			//solucionActual = vecindario;
			for(int j=0; j<data.n; j++)
			{
				solucion->vectorSolucion[j] = vecino->vectorSolucion[j];
			}

			delete[] numerosElegidos;

		}while(n_soluciones < iterRelaTamano*data.n);
	}
	else //BLB
	{
		//int parada = 0;
		bool mejor = false;

		solucion = generarSolucionRandom();
		mejorObjetivo = evaluarSolucion(solucion);
		//sol = generarSolucionRandom();
		do
		{
			mejor = false;

			struct datosSolucionMMDP * vecindario;
			int tam = data.m * (data.n - data.m);

			for(int i=0; i<tam and n_soluciones < iterRelaTamano*data.n; i++)
			{
				vecindario = generarVecino(solucion, i);
				n_soluciones++;
				if( evaluarSolucion(vecindario) > mejorObjetivo)
				{
					mejor = true;
					//solucion = vecindario;
					for(int i=0; i<data.n; i++)
					{
						solucion->vectorSolucion[i] = vecindario->vectorSolucion[i];
					}
					mejorObjetivo = evaluarSolucion(solucion);
				}
			}

		}while((n_soluciones < iterRelaTamano*data.n) && mejor);
	}
	return solucion;
}

struct datosSolucionMMDP * MMDP::aplicarEnfriamientoSimulado(struct datosSolucionMMDP *sol, float tFinal, float gamma, int iterRelaTamano)
{
	struct datosSolucionMMDP * mejorSol;
	mejorSol=reservarMemoriaSolucion();

	struct datosSolucionMMDP * vecindario;
	vecindario=reservarMemoriaSolucion();

	struct datosSolucionMMDP * solucionActual;
	solucionActual=reservarMemoriaSolucion();

	float tInicial = 0.0;
	int tam = data.m*(data.n-data.m);
	float Af = 0.0;
	int n_soluciones = 0, n_movimientos = 0, n = 0;
	bool MejorSolucion = false;
	float t;

	int numeroElegido = 0, i;

	//sol = generarSolucionRandom();

	float x = 0.3;

	tInicial = (10*evaluarSolucion(sol)) / (-log10(x));
	t = tInicial;

	for(i=0; i<data.n; i++)
	{
		solucionActual->vectorSolucion[i] = sol->vectorSolucion[i];
		mejorSol->vectorSolucion[i] = solucionActual->vectorSolucion[i];
	}

	do //bucle externo
	{
		//generacion del aleatorio sin repeticion

		do //bucle interno
		{

			int * numerosPorElegir = new int[tam];
			int * numerosElegidos = new int[tam];

			for(int j=0;j<tam;j++)
			{
				numerosPorElegir[j]=j;
			}

			for (int j=0;j<tam;j++)
			{
				numeroElegido=rand()%(tam-j);

				numerosElegidos[j]=numerosPorElegir[numeroElegido];

				numerosPorElegir[numeroElegido]=numerosPorElegir[tam-j-1];

			}

			delete[] numerosPorElegir;

			MejorSolucion = false;
			vecindario = generarVecino(solucionActual, numerosElegidos[i]);
			i++;

			Af = evaluarSolucion(solucionActual) - evaluarSolucion(vecindario);

			double paceptacion = (double)rand()/RAND_MAX;

			if(Af<0 or (paceptacion) <= (exp(-Af/t)))
			{
				for(int j=0; j<data.n; j++)
					solucionActual->vectorSolucion[j] = vecindario->vectorSolucion[j];

			}

			if(evaluarSolucion(vecindario) > evaluarSolucion(mejorSol))
			{
				MejorSolucion = true;
				for(int j=0; j<data.n; j++)
					mejorSol->vectorSolucion[j] = vecindario->vectorSolucion[j];
				n_movimientos++;
			}
			n_soluciones++;
			n++;
			i=0;
			delete[] numerosElegidos;
		}while(n < data.n and n_movimientos < 0.1*data.n and n_soluciones < iterRelaTamano*data.n);

		//t = enfria(t);
		liberarMemoriaSolucion(vecindario);
		t = t * gamma;

	}while( t > tFinal and (MejorSolucion or n_soluciones < iterRelaTamano*data.n));

	return mejorSol;
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

			//Se inicializa la lista de elecciones posiblesint b=
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
			solucion = reservarMemoriaSolucion();
			for(int i = 0; i < data.m; i++)
			{
				solucion->vectorSolucion[numerosElegidos[i]] = true;
			}

			//hay que generar el vecindario

			return solucion;
}

struct datosSolucionMMDP * MMDP::intercambio(struct datosSolucionMMDP * solucion, int i, int j)
{
	bool auxiliar;

	struct datosSolucionMMDP * aux = new struct datosSolucionMMDP;

	aux=reservarMemoriaSolucion();
	for(int k=0; k<data.n; k++)
	{
		aux->vectorSolucion[k] = solucion->vectorSolucion[k];
	}

	auxiliar = aux->vectorSolucion[i];
	aux->vectorSolucion[i] = aux->vectorSolucion[j];
	aux->vectorSolucion[j] = auxiliar;

	return aux;
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
