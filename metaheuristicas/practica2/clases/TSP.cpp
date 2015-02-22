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
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

namespace clases
{
	//CONSTRUCTORES

	TSP::TSP()
	{
		data.m=0;
		tipo = NULL;
	}

	TSP::~TSP()
	{
		liberarMemoria();
	}
	//RESERVA DE MEMORIA

	int TSP::reservarMemoria()
	{

		data.distancias = new float * [data.m];

		for(int i=0; i<data.m; i++)
		{
			data.distancias[i] = new float [data.m];
		}

		return 0;
	}

	datosSolucionTSP * TSP::reservarMemoriaSolucion()
	{
		struct datosSolucionTSP * solucion = new struct datosSolucionTSP;
		solucion->vectorSolucion = new int [data.m];

		for(int i=0; i<data.m; i++)
		{
			solucion->vectorSolucion[i] = -1;
		}

		return solucion;
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

		//TRABAJAR CON LA INSTANCIA

	void TSP::liberarMemoriaSolucion(struct datosSolucionTSP *sol)
	{
		delete[] sol->vectorSolucion;
		delete sol;
	}

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
		float valor=0.0;
		for(int i=0; i<data.m; i++)
		{
			for(int j=0; j<data.m; j++)
			{
				fichero >> valor;
				data.distancias[i][j]=valor;
				//data.distancias[j][i]=valor;
			}
		}

		fichero.close();

		return 1;
	}

	//TRABAJAR CON LA SOLUCION

	int * TSP::CVF(struct datosSolucionTSP * sol)
	{
		int i;
		std::vector <float> auxiliar; //esto para ordenar las distancias

		for(i=0; i<data.m-1; i++)//hay que poner -1 sino el ultimo elemento se va a salir de la matriz
		{
			auxiliar.push_back(data.distancias[sol->vectorSolucion[i]][sol->vectorSolucion[i+1]]);
		}

		auxiliar.push_back(data.distancias[sol->vectorSolucion[data.m-1]][sol->vectorSolucion[0]]);
		//esto es para añadir la distancia del ultimo con el primero

		std::vector <float> original(auxiliar);

		sort(auxiliar.rbegin(), auxiliar.rend());

		//comprobado que ordenar con rbegin y rend ordena bien

		/*cout << "===========================================" << endl;
		cout << "Este es el vector distancias original: " << endl;
		for(unsigned int i=0; i<original.size(); i++)
		{
			cout << original[i] << " ";
		}
		cout << endl;
		cout << "===========================================" << endl;

		cout << "===========================================" << endl;
		cout << "Este es el vector distancias ya ordenado: " << endl;
		for(unsigned int i=0; i<auxiliar.size(); i++)
		{
			cout << auxiliar[i] << " ";
		}
		cout << endl;
		cout << "===========================================" << endl;*/

		int * CV = new int[data.m];

		for(int i=0; i<data.m; i++)
		{
			//con este for voy a recorrer el vector de las distancias auxiliar
			//por cada elemento de este vector hay que buscar la posicion que le corresponde
			//en el vector de distancias original
			for(unsigned int j=0; j<original.size(); j++)
			{
				//con este recorremos el original. Si encontramos el que buscamos del ordenado
				//se guarda el indice del original en el vector CV
				if(original[j] == auxiliar[i])
				{
					CV[i] = j;
					//k++;
				}
			}
		}

		/*cout << "Completado el CV" << endl;

		for(int i=0; i<tam; i++)
		{
			cerr << CV[i] << " ";
		}
		cout << endl;*/

		original.clear();
		auxiliar.clear();

		return CV;
	}

	struct datosSolucionTSP * TSP::generarVecino(struct datosSolucionTSP* sol, int indice, int tipo)
	{
		struct datosSolucionTSP * solucion;
		solucion=reservarMemoriaSolucion();

		struct datosSolucionTSP * auxiliar;
		auxiliar = reservarMemoriaSolucion();

		struct datosSolucionTSP * auxstr;
		auxstr=reservarMemoriaSolucion();

		for(int i=0; i<data.m; i++)
		{
			solucion->vectorSolucion[i] = sol->vectorSolucion[i];
			auxstr->vectorSolucion[i]=sol->vectorSolucion[i];
		}

		//elementos de la factorizacion
		int x0=0, x1=0, x2=0, y0=0, y1=0, y2=0, xp0=0, xp1=0, xp2=0, yp0=0, yp1=0, yp2=0;
		float suma=0.0, resta=0.0, resultado=0.0, resultado2 = 0;
		int ii,jj;

		resultado = evaluarSolucion(solucion);

		int * CV = new int [data.m];
		CV = CVF(solucion);

		for(int i=0; i<data.m; i++)
		{
			auxiliar->vectorSolucion[i] = CV[i];
		}

		if(tipo == 1) //es blb
		{
			for(int j=0; j<data.m; j++)
			{
				if(j != 0)
				{
					if( (CV[indice] != CV[j]) && (CV[(indice-1)%data.m] != CV[j]) && (CV[(indice+1)%data.m] != CV[j]))
					{

						if(CV[j] < CV[indice]){
							ii = CV[j];
							jj = CV[indice];
						}
						else{
							ii = CV[indice];
							jj = CV[j];
						}

						auxiliar=opt(solucion,ii,jj);

						x0=solucion->vectorSolucion[ii];//i
						x1=solucion->vectorSolucion[ii+1];//i+1
						x2=solucion->vectorSolucion[ii+2];//i+2
						y0=solucion->vectorSolucion[jj];//j
						y1=solucion->vectorSolucion[(jj+1)%data.m];//j+1
						y2=solucion->vectorSolucion[jj-1];//j-1

						xp0=auxiliar->vectorSolucion[ii];//i'
						xp1=auxiliar->vectorSolucion[(ii+1)%data.m];//i'+1
						xp2=auxiliar->vectorSolucion[(ii+2)%data.m];//i'+2
						yp0=auxiliar->vectorSolucion[jj];//j'
						yp1=auxiliar->vectorSolucion[(jj+1)%data.m];//j'+1
						yp2=auxiliar->vectorSolucion[jj-1];//j'-1

						suma=data.distancias[xp0][xp1]+data.distancias[xp1][xp2]+data.distancias[yp2][yp0]+data.distancias[yp0][yp1];

						resta=data.distancias[(y2)%data.m][y0%data.m]+data.distancias[y0%data.m][y1%data.m]+data.distancias[x0%data.m][x1%data.m]+data.distancias[x1%data.m][x2%data.m];


						resultado2 = resultado + suma - resta;

						if(resultado > resultado2)
						{
							for(int i=0; i<data.m; i++)
							{
								auxstr->vectorSolucion[i] = auxiliar->vectorSolucion[i];
							}
							resultado = resultado2;
						}
						liberarMemoriaSolucion(auxiliar);
					}
				}
				else
				{
					if( (CV[indice] != CV[j]) && (CV[(indice-1)%data.m] != CV[j]) && (CV[(indice+1)%data.m] != CV[j]))
					{

						int ii,jj;
						if(CV[j] < CV[indice]){
							ii = CV[j];
							jj = CV[indice];
						}
						else{
							ii = CV[indice];
							jj = CV[j];
						}

						auxiliar=opt(solucion,ii,jj);

						x0=solucion->vectorSolucion[ii];//i
						x1=solucion->vectorSolucion[ii+1];//i+1
						x2=solucion->vectorSolucion[ii+2];//i+2
						y0=solucion->vectorSolucion[jj];//j
						y1=solucion->vectorSolucion[(jj+1)%data.m];//j+1
						y2=solucion->vectorSolucion[jj-1];//j-1

						xp0=auxiliar->vectorSolucion[ii];//i'
						xp1=auxiliar->vectorSolucion[(ii+1)%data.m];//i'+1
						xp2=auxiliar->vectorSolucion[(ii+2)%data.m];//i'+2
						yp0=auxiliar->vectorSolucion[jj];//j'
						yp1=auxiliar->vectorSolucion[(jj+1)%data.m];//j'+1
						yp2=auxiliar->vectorSolucion[jj-1];//j'-1

						suma=data.distancias[xp0][xp1]+data.distancias[xp1][xp2]+data.distancias[yp2][yp0]+data.distancias[yp0][yp1];

						resta=data.distancias[(y2)%data.m][y0%data.m]+data.distancias[y0%data.m][y1%data.m]+data.distancias[x0%data.m][x1%data.m]+data.distancias[x1%data.m][x2%data.m];

						resultado2 = resultado + suma - resta;

						if(resultado > resultado2)
						{
							for(int i=0; i<data.m; i++)
							{
								auxstr->vectorSolucion[i] = auxiliar->vectorSolucion[i];
							}
							resultado = resultado2;
						}
						liberarMemoriaSolucion(auxiliar);

					}
				}
			}//for
		}

		else if (tipo == 2)//es blf
		{
			int * numerosPorElegir = new int[data.m];
			int * numerosElegidos = new int[data.m];
			int numeroElegido=0;
			int k=0;

			for(int j=0;j<data.m;j++)
			{
				numerosPorElegir[j]=j;
			}

			for (int j=0;j<data.m;j++)
			{
				numeroElegido =rand()%(data.m-j);

				numerosElegidos[j]=numerosPorElegir[numeroElegido];

				numerosPorElegir[numeroElegido]=numerosPorElegir[data.m-j-1];
			}

			delete [] numerosPorElegir;

			int j = numerosElegidos[k];
			k++;

			if(j != 0)
			{
				if( (CV[indice] != CV[j]) && (CV[(indice-1)%data.m] != CV[j]) && (CV[(indice+1)%data.m] != CV[j]))
				{

					if(CV[j] < CV[indice]){
						ii = CV[j];
						jj = CV[indice];
					}
					else{
						ii = CV[indice];
						jj = CV[j];
					}

					auxiliar=opt(solucion,ii,jj);

					x0=solucion->vectorSolucion[ii];//i
					x1=solucion->vectorSolucion[ii+1];//i+1
					x2=solucion->vectorSolucion[ii+2];//i+2
					y0=solucion->vectorSolucion[jj];//j
					y1=solucion->vectorSolucion[(jj+1)%data.m];//j+1
					y2=solucion->vectorSolucion[jj-1];//j-1

					xp0=auxiliar->vectorSolucion[ii];//i'
					xp1=auxiliar->vectorSolucion[(ii+1)%data.m];//i'+1
					xp2=auxiliar->vectorSolucion[(ii+2)%data.m];//i'+2
					yp0=auxiliar->vectorSolucion[jj];//j'
					yp1=auxiliar->vectorSolucion[(jj+1)%data.m];//j'+1
					yp2=auxiliar->vectorSolucion[jj-1];//j'-1

					suma=data.distancias[xp0][xp1]+data.distancias[xp1][xp2]+data.distancias[yp2][yp0]+data.distancias[yp0][yp1];

					resta=data.distancias[(y2)%data.m][y0%data.m]+data.distancias[y0%data.m][y1%data.m]+data.distancias[x0%data.m][x1%data.m]+data.distancias[x1%data.m][x2%data.m];


					resultado2 = resultado + suma - resta;

					if(resultado > resultado2)
					{
						for(int i=0; i<data.m; i++)
						{
							auxstr->vectorSolucion[i] = auxiliar->vectorSolucion[i];
						}
						resultado = resultado2;
					}
					liberarMemoriaSolucion(auxiliar);
				}
			}
			else
			{
				if( (CV[indice] != CV[j]) && (CV[(indice-1)%data.m] != CV[j]) && (CV[(indice+1)%data.m] != CV[j]))
				{

					int ii,jj;
					if(CV[j] < CV[indice]){
						ii = CV[j];
						jj = CV[indice];
					}
					else{
						ii = CV[indice];
						jj = CV[j];
					}

					auxiliar=opt(solucion,ii,jj);

					x0=solucion->vectorSolucion[ii];//i
					x1=solucion->vectorSolucion[ii+1];//i+1
					x2=solucion->vectorSolucion[ii+2];//i+2
					y0=solucion->vectorSolucion[jj];//j
					y1=solucion->vectorSolucion[(jj+1)%data.m];//j+1
					y2=solucion->vectorSolucion[jj-1];//j-1

					xp0=auxiliar->vectorSolucion[ii];//i'
					xp1=auxiliar->vectorSolucion[(ii+1)%data.m];//i'+1
					xp2=auxiliar->vectorSolucion[(ii+2)%data.m];//i'+2
					yp0=auxiliar->vectorSolucion[jj];//j'
					yp1=auxiliar->vectorSolucion[(jj+1)%data.m];//j'+1
					yp2=auxiliar->vectorSolucion[jj-1];//j'-1

					suma=data.distancias[xp0][xp1]+data.distancias[xp1][xp2]+data.distancias[yp2][yp0]+data.distancias[yp0][yp1];

					resta=data.distancias[(y2)%data.m][y0%data.m]+data.distancias[y0%data.m][y1%data.m]+data.distancias[x0%data.m][x1%data.m]+data.distancias[x1%data.m][x2%data.m];

					resultado2 = resultado + suma - resta;

					if(resultado > resultado2)
					{
						for(int i=0; i<data.m; i++)
						{
							auxstr->vectorSolucion[i] = auxiliar->vectorSolucion[i];
						}
						resultado = resultado2;
					}
					liberarMemoriaSolucion(auxiliar);

				}
			}
		}

		else if (tipo == 3)// es ES
		{
			int j = 0;
			j = rand()%data.m;
			//cerr << j << endl;
			//cerr << "Hola vamos a hacer el ES" << endl;
			if(j != 0)
			{
				if( (CV[indice] != CV[j]) && (CV[(indice-1)%data.m] != CV[j]) && (CV[(indice+1)%data.m] != CV[j]))
				{

					if(CV[j] < CV[indice]){
						ii = CV[j];
						jj = CV[indice];
					}
					else{
						ii = CV[indice];
						jj = CV[j];
					}

					auxiliar=opt(solucion,ii,jj);

					x0=solucion->vectorSolucion[ii];//i
					x1=solucion->vectorSolucion[ii+1];//i+1
					x2=solucion->vectorSolucion[ii+2];//i+2
					y0=solucion->vectorSolucion[jj];//j
					y1=solucion->vectorSolucion[(jj+1)%data.m];//j+1
					y2=solucion->vectorSolucion[jj-1];//j-1

					xp0=auxiliar->vectorSolucion[ii];//i'
					xp1=auxiliar->vectorSolucion[(ii+1)%data.m];//i'+1
					xp2=auxiliar->vectorSolucion[(ii+2)%data.m];//i'+2
					yp0=auxiliar->vectorSolucion[jj];//j'
					yp1=auxiliar->vectorSolucion[(jj+1)%data.m];//j'+1
					yp2=auxiliar->vectorSolucion[jj-1];//j'-1

					suma=data.distancias[xp0][xp1]+data.distancias[xp1][xp2]+data.distancias[yp2][yp0]+data.distancias[yp0][yp1];

					resta=data.distancias[(y2)%data.m][y0%data.m]+data.distancias[y0%data.m][y1%data.m]+data.distancias[x0%data.m][x1%data.m]+data.distancias[x1%data.m][x2%data.m];


					resultado2 = resultado + suma - resta;

					if(resultado > resultado2)
					{
						for(int i=0; i<data.m; i++)
						{
							auxstr->vectorSolucion[i] = auxiliar->vectorSolucion[i];
						}
						resultado = resultado2;
					}
					liberarMemoriaSolucion(auxiliar);
				}
			}
			else
			{
				if( (CV[indice] != CV[j]) && (CV[(indice-1)%data.m] != CV[j]) && (CV[(indice+1)%data.m] != CV[j]))
				{

					int ii,jj;
					if(CV[j] < CV[indice]){
						ii = CV[j];
						jj = CV[indice];
					}
					else{
						ii = CV[indice];
						jj = CV[j];
					}
					/*if(jj == ii+1){
						cout << "Esto no debería producirse" << endl;
					}*/
					auxiliar=opt(solucion,ii,jj);

					x0=solucion->vectorSolucion[ii];//i
					x1=solucion->vectorSolucion[ii+1];//i+1
					x2=solucion->vectorSolucion[ii+2];//i+2
					y0=solucion->vectorSolucion[jj];//j
					y1=solucion->vectorSolucion[(jj+1)%data.m];//j+1
					y2=solucion->vectorSolucion[jj-1];//j-1

					xp0=auxiliar->vectorSolucion[ii];//i'
					xp1=auxiliar->vectorSolucion[(ii+1)%data.m];//i'+1
					xp2=auxiliar->vectorSolucion[(ii+2)%data.m];//i'+2
					yp0=auxiliar->vectorSolucion[jj];//j'
					yp1=auxiliar->vectorSolucion[(jj+1)%data.m];//j'+1
					yp2=auxiliar->vectorSolucion[jj-1];//j'-1

					suma=data.distancias[xp0][xp1]+data.distancias[xp1][xp2]+data.distancias[yp2][yp0]+data.distancias[yp0][yp1];

					resta=data.distancias[(y2)%data.m][y0%data.m]+data.distancias[y0%data.m][y1%data.m]+data.distancias[x0%data.m][x1%data.m]+data.distancias[x1%data.m][x2%data.m];

					resultado2 = resultado + suma - resta;

					if(resultado > resultado2)
					{
						for(int i=0; i<data.m; i++)
						{
							auxstr->vectorSolucion[i] = auxiliar->vectorSolucion[i];
						}
						resultado = resultado2;
					}
					liberarMemoriaSolucion(auxiliar);

				}
			}
		}

		delete[] CV;

		return auxstr;
	}

	struct datosSolucionTSP * TSP::opt(struct datosSolucionTSP * solucion, int indice1, int indice2)
	{
		struct datosSolucionTSP * auxiliar;
		auxiliar=reservarMemoriaSolucion();
			for (int i=0;i<data.m;i++)
			{
				auxiliar->vectorSolucion[i]=solucion->vectorSolucion[i];
			}

			float aux;

			aux=auxiliar->vectorSolucion[(indice1+1)%data.m];
			auxiliar->vectorSolucion[(indice1+1)%data.m]=auxiliar->vectorSolucion[indice2];
			auxiliar->vectorSolucion[indice2]=aux;
			return auxiliar;

		return auxiliar;
	}

	struct datosSolucionTSP * TSP::aplicarBusquedaLocal(struct datosSolucionTSP *sol,  bool firstImprovement, int iterRelaTamano)
	{
		int tam = data.m * 0.5;
		int indice = 0;
		int n_soluciones=0;
		float valorActual = 0.0, mejorObjetivo = 0.0;
		int numeroElegido = 0;

		struct datosSolucionTSP * solucion = new struct datosSolucionTSP;
		solucion = reservarMemoriaSolucion();

		struct datosSolucionTSP * vecino = new struct datosSolucionTSP;
		vecino = reservarMemoriaSolucion();

		for(int i=0; i<data.m; i++)
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
					vecino = generarVecino(solucion, indice, 2);
					n_soluciones++;
					i++;

				}while(evaluarSolucion(vecino) > valorActual);

				i=0;

				//solucionActual = vecindario;
				for(int j=0; j<data.m; j++)
				{
					solucion->vectorSolucion[j] = vecino->vectorSolucion[j];
				}

				delete[] numerosElegidos;

			}while(n_soluciones < iterRelaTamano*data.m);
		}
		else //BLB
		{
			//int parada = 0;
			bool mejor = false;

			//solucion = generarSolucionRandom();
			mejorObjetivo = evaluarSolucion(solucion);

			do
			{
				mejor = false;

				for(int i=0; i<data.m and (n_soluciones < iterRelaTamano); i++)
				{
					vecino = generarVecino(solucion, i, 1);
					n_soluciones++;

					//cout << evaluarSolucion(vecino) << endl;

					if(evaluarSolucion(vecino) < mejorObjetivo) //peta aqui
					{
						mejor = true;
						//solucion = vecindario;
						for(int i=0; i<data.m; i++)
						{
							solucion->vectorSolucion[i] = vecino->vectorSolucion[i];
						}
						mejorObjetivo = evaluarSolucion(solucion);
					}
					//liberarMemoriaSolucion(vecino);
				}
			}while((n_soluciones < iterRelaTamano*data.m) && mejor);
		}

		return solucion;
	}

	struct datosSolucionTSP * TSP::aplicarEnfriamientoSimulado(struct datosSolucionTSP *sol, float tFinal, float gamma, int iterRelaTamano)
	{
		struct datosSolucionTSP * mejorSol;
		mejorSol=reservarMemoriaSolucion();

		struct datosSolucionTSP * vecindario;
		vecindario=reservarMemoriaSolucion();

		struct datosSolucionTSP * solucionActual;
		solucionActual=reservarMemoriaSolucion();

		float tInicial = 0.0;
		int tam = data.m * 0.5;
		float Af = 0.0;
		int n_soluciones = 0, n_movimientos = 0, n = 0;
		bool MejorSolucion = false;
		float t;

		int numeroElegido = 0, i=0;

		//sol = generarSolucionRandom();

		float x = 0.3;

		tInicial = (0.001*evaluarSolucion(sol)) / (-log10(x));
		t = tInicial;

		for(i=0; i<data.m; i++)
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
				cerr << evaluarSolucion(solucionActual) << " " << numerosElegidos[i] << endl;
				vecindario = generarVecino(solucionActual, numerosElegidos[i], 3);
				i++;

				Af = evaluarSolucion(vecindario) - evaluarSolucion(solucionActual);

				double paceptacion = (double)rand()/RAND_MAX;

				if(Af<0 or (paceptacion) <= (exp(-Af/t)))
				{
					for(int j=0; j<data.m; j++)
						solucionActual->vectorSolucion[j] = vecindario->vectorSolucion[j];

				}

				if(evaluarSolucion(vecindario) < evaluarSolucion(mejorSol))
				{
					MejorSolucion = true;
					for(int j=0; j<data.m; j++)
						mejorSol->vectorSolucion[j] = vecindario->vectorSolucion[j];
					n_movimientos++;
				}
				n_soluciones++;
				n++;
				i=0;
				delete[] numerosElegidos;
			}while(n < data.m*5 and n_movimientos < 0.1*data.m and n_soluciones < iterRelaTamano*data.m);

			//t = enfria(t);
			liberarMemoriaSolucion(vecindario);
			t = t * gamma;

		}while( t > tFinal and (MejorSolucion or n_soluciones < iterRelaTamano*data.m));

		return mejorSol;
	}


	float TSP::evaluarSolucion(struct datosSolucionTSP* sol)
	{
		struct  datosSolucionTSP * solucion;
		solucion = reservarMemoriaSolucion();

		for(int i=0; i<data.m; i++)
		{
			solucion->vectorSolucion[i] = sol->vectorSolucion[i];
		}

		float distancia=0;
		int i=0;
		for(i=0; i<data.m-1; i++)
		{
			distancia += data.distancias[solucion->vectorSolucion[i]][solucion->vectorSolucion[i+1]];
		}

		distancia += data.distancias[data.m-1][solucion->vectorSolucion[0]];

		return distancia;
	}

	struct datosSolucionTSP * TSP::generarSolucionRandom()
	{
		//Se reserva memoria del vector solucion
		struct datosSolucionTSP * solucion = new struct datosSolucionTSP;
		solucion = reservarMemoriaSolucion();

		int i=0;
		int posicion=0;
		bool repetido = false;
		while(i<data.m) //este while es para generar los indices aleatorios
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
		};

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
