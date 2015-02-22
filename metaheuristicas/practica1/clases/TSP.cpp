/*
 * TSP.cpp
 *
 *  Created on: 05/03/2014
 *      Author: i12luqqd
 */

#include "TSP.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

namespace clases
{
	//CONSTRUCTORES

	TSP::TSP()
	{
		data.m=0;
	}

	TSP::~TSP()
	{
		liberarMemoria();
	}

	//RESERVA DE MEMORIA

	int TSP::reservarMemoria()
	{

		data.distancias = new int * [data.m];

		for(int i=0; i<data.m; i++)
		{
			data.distancias[i] = new int [data.m];
		}

		return 0;
	}

	int TSP::reservarMemoriaSolucion(struct datosSolucionTSP * solucion)
	{
		solucion->vectorSolucion = new int [data.m];

		for(int i=0; i<data.m; i++)
		{
			solucion->vectorSolucion[i] = -1;
		}

		return 0;
	}

	//LIBERACION DE MEMORIA

	void TSP::liberarMemoria()
	{
		for(int i=0; i<data.m; i++)
		{
			delete[] data.distancias[i];
		}
		delete[] data.distancias;
	}

	void TSP::liberarMemoriaSolucion(struct datosSolucionTSP *sol)
	{
		delete[] sol->vectorSolucion;
		delete sol;
	}

	//TRABAJAR CON LA INSTANCIA

	int TSP::leerInstancia(char *archivo)
	{
		//abrimos el fichero
		ifstream fichero;
		fichero.open(archivo);

		//cogemos la primera linea para saber m
		fichero >> data.m;

		//reservamos la memoria
		int memoria;
		memoria = reservarMemoria();

		if(memoria!=0)
		{
			exit(-1);
		}

		//terminamos de leer la instancia
		int valor=0;
		for(int i=0; i<data.m; i++)
		{
			for(int j=0; j<data.m; j++)
			{
				fichero >> valor;
				data.distancias[i][j]=valor;
			}
		}

		fichero.close();

		return 1;
	}

	//TRABAJAR CON LA SOLUCION

	float TSP::evaluarSolucion(struct datosSolucionTSP* sol)
	{
		float distancia=0;
		int i=0;
		for(i=0; i<data.m; i++)
		{
			distancia += data.distancias[sol->vectorSolucion[i]][sol->vectorSolucion[i+1]];
		}

		distancia += data.distancias[data.m-1][sol->vectorSolucion[0]];

		return distancia;
	}

	struct datosSolucionTSP * TSP::generarSolucionRandom()
	{
		//Reserva de memoria de los vectores de numeros
		/*int * numerosPorElegir = new int [data.m];
		int * numerosElegidos = new int [data.m];

		//Se inicializa la lista de elecciones posibles
		for(int i = 0; i < data.m; i++)
		{
			numerosPorElegir[i] = i;
		}

		for(int i = 0; i < data.m; i++)
		{
			int numeroElegido = rand()%(data.m-i);
			numerosElegidos[i] = numerosPorElegir[numeroElegido];//Recordar el numero elegido
			numerosPorElegir[numeroElegido] = numerosPorElegir[data.m-i-1];//Se ponen en numeros por elegir, el ultimo que era valido, asi son todos validos hasta n-i-1
		}

		//Se libera la memoria del vector que ya no se usara
		delete [] numerosPorElegir;*/

		//Se reserva memoria del vector solucion
		struct datosSolucionTSP * solucion = new struct datosSolucionTSP;
		reservarMemoriaSolucion(solucion);


		int i=0;
		int posicion=0;
		bool repetido = false;
		while(i<data.m)
		{
			repetido = false;
			posicion=rand()%data.m;
			for(int j=0; j<data.m; j++)
			{
				if(solucion->vectorSolucion[j] == posicion)
				{
					repetido = true;
					i--;
				}
			}
			if(solucion->vectorSolucion[i] == -1 && repetido == false)
			{
				solucion->vectorSolucion[i]=posicion;
			}
			i++;
		}

		/*for(int i = 0; i < data.m; i++)
			{
				solucion->vectorSolucion[i] = numerosElegidos[i];
			}*/

		//delete[] numerosElegidos;

		return solucion;
	}

	//IMPRIMIR

	void TSP::imprimirSolucion(struct datosSolucionTSP * sol)
	{
		cout << "Vector solucion" << endl;
		for(int i=0; i<data.m; i++)
		{
			cout << sol->vectorSolucion[i] << " ";
		}
		cout << endl;
	}

	void TSP::imprimirInstancia()
	{
		for(int i=0; i<data.m; i++)
		{
			for(int j=0; j<data.m; j++)
			{
				cout << "\t" << data.distancias[i][j];
			}
			cout << endl;
		}
	}
}
