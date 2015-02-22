/*
 * CPH.cpp
 *
 *  Created on: 11/03/2014
 *      Author: i12luqqd
 */

#include "CPH.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>

using namespace std;
namespace clases
{
	//CONSTRUCTORES

	CPH::CPH()
	{
		data.n = 0;
		data.p = 0;
		data.c = 0;
	}

	CPH::~CPH()
	{
		liberarMemoria();
	}

	//RESERVA DE MEMORIA

	int CPH::reservarMemoria()
	{
		data.cooNodo = new struct coordenadas [data.n];


		for(int i= 0; i<data.n; i++)
		{
				data.cooNodo[i].f = 0.0;
				data.cooNodo[i].x = 0.0;
				data.cooNodo[i].y = 0.0;
		}

		//Para la matriz distancia

		data.distancias = new float * [data.n];

		for(int i=0; i<data.n; i++)
		{
			data.distancias[i] = new float [data.n];
		}

		return 1;
	}

	int CPH::reservarMemoriaSolucion(struct datosSolucionCPH * solucion)
	{

		//matrizConectividad

		solucion->matrizConectividad = new bool * [data.n];

		for(int i=0; i<data.n; i++)
		{
			solucion->matrizConectividad[i] = new bool [data.n];
		}

		for(int i=0; i<data.n; i++)
		{
			for(int j=0; j<data.n; j++)
			{
				solucion->matrizConectividad[i][j] = false;
			}
		}

		//vectorSolucion

		solucion->vectorSolucion = new int [data.n];

		for(int i=0; i<data.n; i++)
		{
			solucion->vectorSolucion[i] = -1;
		}

		return 1;
	}

	//LIBERACION DE MEMORIA

	void CPH::liberarMemoria()
	{
		delete[] data.cooNodo;
	}

	void CPH::liberarMemoriaSolucion(struct datosSolucionCPH * sol)
	{
		for(int i=0; i<data.n; i++)
		{
			delete[] sol->matrizConectividad[i];
		}
		delete[] sol->matrizConectividad;

		delete[] sol->vectorSolucion;
	}

	//TRABAJAR CON LA INSTANCIA

	int CPH::leerInstancia(char * archivo)
	{
		//abrimos el fichero
		ifstream fichero;
		fichero.open(archivo);

		//leemos la primera linea en la que cogemos el numero de nodos (n), numero de concentradores (p) y capacidad (c)
		fichero >> data.n >> data.p >> data.c;

		//reservamos la memoria
		int memoria = 0;
		memoria = reservarMemoria();
		if(memoria != 1)
		{
			exit(-1);
		}

		//ahora leemos el resto del fichero
		int i=0;
		float x, y, f;
		while(!fichero.eof())
		{
			fichero >> i >> x >> y >> f;
			data.cooNodo[i-1].x = x;
			data.cooNodo[i-1].y = y;
			data.cooNodo[i-1].f = f;
		}

		//ahora calculamos la distancia
		float distancia = 0;
		for(int i = 0; i < data.n; i++)
			{
				for(int j = i+1; j < data.n; j++)
				{
					distancia = sqrt(pow((data.cooNodo[i].x - data.cooNodo[j].x),2) + pow((data.cooNodo[i].y - data.cooNodo[j].y),2));
					data.distancias[i][j] = data.distancias[j][i] = distancia;
				}
			}

		fichero.close();

		return 1;
	}

	//TRABAJAR CON LA SOLUCION

	float CPH::evaluarSolucion(struct datosSolucionCPH * sol)
	{
		float distancia=0;
		for(int i=0; i<data.n; i++)
		{
			distancia += data.distancias[i][sol->vectorSolucion[i]];
		}
		return distancia;
		return 1;
	}

	struct datosSolucionCPH * CPH::generarSolucionRandom()
	{

		struct datosSolucionCPH * solucion = new struct datosSolucionCPH;
		reservarMemoriaSolucion(solucion);



		//esto es para generar los concentradores en el vector de solucion

		for(int i=data.p; i<data.p*2; i++)
		{
			solucion->vectorSolucion[rand()%data.n] = i;
		}

		int concentradores[data.p]; //para guardar la demanda de los concentradores

		for(int i=0;i<data.p;i++)
		{
			concentradores[i] = 0;
		}

		int minimo=0;
		int indice=0;

		for(int i=0; i<data.n; i++)
		{
			if(solucion->vectorSolucion[i] < data.p) //es cliente
			{
				minimo = 999;
				for(int j=0; j<data.p; j++)
				{
					if(concentradores[j] < minimo)
					{
						minimo = concentradores[j];
						indice = j;
					}
				}
				solucion->vectorSolucion[i] = indice;
				//cout << "Demanda: " << data.cooNodo[i]->f << endl;
				concentradores[indice] += data.cooNodo[i].f;

			}
		}

		return solucion;
	}

	//IMPRIMIR

	void CPH::imprimirSolucion(struct datosSolucionCPH * sol)
	{
		cout << "Este el vectorSolucion" << endl;
		for(int i=0; i<data.n; i++)
		{
			cout << sol->vectorSolucion[i] << " ";
		}
		cout << endl;
	}

	void CPH::imprimirInstancia()
	{
		cout << "Datos de la instancia";
		cout << "\t" << data.c << "\t" << data.n << "\t" << data.p<< endl;
		cout << "Esta es la matriz de distancias" << endl;
		for(int i=0; i<data.n; i++)
		{
			for(int j=0; j<data.n; j++)
			{
				cout << data.distancias[i][j] << " ";
			}
			cout << endl;
		}
	}
}
