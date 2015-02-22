/*
 * CWP.cpp
 *
 *  Created on: 09/03/2014
 *      Author: david
 */

#include "CWP.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;
namespace clases
{

	//CONSTRUCTORES
	CWP::CWP()
	{
		data.n=0;
		data.m=0;
	}

	CWP::~CWP()
	{
		liberarMemoria();
	}

	//RESERVA DE MEMORIA

	int CWP::reservarMemoria()
	{

		data.distancias = new bool * [data.n];

		for(int i=0; i<data.n; i++)
		{
			data.distancias[i] = new bool [data.n];
		}

		return 1;
	}

	int CWP::reservarMemoriaSolucion(struct datosSolucionCWP * solucion)
	{
		solucion->vectorSolucion = new int [data.n];

		for(int i=0; i<data.n; i++)
		{
			solucion->vectorSolucion[i] = -1;
		}

		return 1;
	}

	//LIBERACION DE MEMORIA
	void CWP::liberarMemoria()
	{
		for(int i=0; i<data.n; i++)
		{
			delete[] data.distancias[i];
		}

		delete[] data.distancias;
	}

	void CWP::liberarMemoriaSolucion(struct datosSolucionCWP *sol)
	{
		delete[] sol->vectorSolucion;
	}

	//TRABAJAR C0N LA INSTANCIA

	int CWP::leerInstancia(char *archivo)
	{

		//primero creamos el flujo y abrimos el archivo
		ifstream fichero;
		fichero.open(archivo);

		//leemos la primera linea teniendo en cuenta que la n esta repetida
		fichero >> data.n >> data.n >> data.m;

		//ahora vamos a leer los indices y en los que indiquen colocaremos un true en la matriz de distancias
		int i=0, j=0;

		int memoria = 0;
		memoria = reservarMemoria();
		if(memoria != 1)
		{
			exit(-1);
		}

		while(!fichero.eof())
		{
			fichero >> i >> j;
			data.distancias[i-1][j-1] = true;
			data.distancias[j-1][i-1] = true;
		}

		fichero.close();

		return 1;
	}

	//TRABAJAR CON LA SOLUCION

	float CWP::evaluarSolucion(struct datosSolucionCWP * sol)
	{
		float contador=0.0;
		float vector[data.n];
		float maximo = 0;
		for(int i=0; i<data.n; i++)
		{
			contador=0;
			for(int j=0; j<i+1; j++)
			{
				for(int k=i+1; k<data.n; k++)
				{
					if(data.distancias[sol->vectorSolucion[j]][sol->vectorSolucion[k]] == true)
					{
						contador++;
					}
				}
			}
			vector[i] = contador;
		}

		for(int i=0; i<data.n; i++)
		{
			if(vector[i] > maximo)
			{
				maximo = vector[i];
				cout << maximo << " ";
			}
		}
		cout << endl;

		return maximo;
	}

	struct datosSolucionCWP * CWP::generarSolucionRandom()
	{

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

		//en numerosElegidos tenemos una posible ordenacion del grafo.

		struct datosSolucionCWP * solucion = new struct datosSolucionCWP;
		reservarMemoriaSolucion(solucion);

		for(int i=0; i<data.n; i++)
		{
			solucion->vectorSolucion[i] = numerosElegidos[i];
		}

		imprimirSolucion(solucion);

		return solucion;
	}

	//IMPRIMIR

	void CWP::imprimirSolucion(struct datosSolucionCWP * sol)
	{
		cout << "\n****************" << endl;
		cout << "Vector solucion" << endl;
		cout << "****************" << endl;
		for(int i=0; i<data.n; i++)
		{
			cout << sol->vectorSolucion[i] << " ";
		}
		cout << endl;
		cout << "-----------------------------------" << endl;
	}

	void CWP::imprimirInstancia()
	{
		for(int i=0; i<data.n; i++)
		{
			for(int j=0; j<data.n; j++)
			{
				cout << "\t" << data.distancias[i][j] << " ";
			}
			cout << endl;
		}
	}
}


