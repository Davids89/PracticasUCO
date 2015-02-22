/*
 * CPH.cpp
 *
 *  Created on: 11/03/2014
 *      Author: i12luqqd
 */

#include "CPH.h"
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <new>
#include <cfloat> //para el FLT_MAX

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

		/*for(int i=0; i<data.n; i++)
		{
			solucion->vectorSolucion[i] = -1;
		}*/

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
		int cliente=0;

		int demandas[data.p];
		for(int i=0; i<data.p; i++)
		{
			demandas[i] = 0;
		}

		//lo siguiente es para calcular la demanda

		for(int k=0;k<data.p;k++)
		{
			for(int i=0;i<data.n;i++)
			{
				if(sol->vectorSolucion[i]==cliente or sol->vectorSolucion[i]==(data.p+cliente))
				{
					demandas[k] += data.cooNodo[i].f;
				}
			}
			cliente++;
		 }

		//ahora hay que comprobar que no se sobrepasa la demanda de ningun concentrador
		for(int i=0;i<data.p;i++) //por si se desborda en la demanda algun concentrador
		{
			if(demandas[i]>data.c)
			{
				distancia=FLT_MAX;
			}
		}

		if(distancia == 0.0)
		{
			for(int i=0; i<data.n; i++)
			{
				if(sol->vectorSolucion[i] < data.p){
					int indiceConcentrador=0;
					for(int j=0; j< data.n; j++)
						if(sol->vectorSolucion[j] == data.p + sol->vectorSolucion[i])
							indiceConcentrador=j;
					distancia += data.distancias[i][indiceConcentrador];
				}
			}
		}	
		return distancia;
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

	void CPH::operadorMutacion(struct datosSolucionCPH *ind)
	{
		//tengo que crear dos indices al azar e intercambiar el contenido de ind->vectorSolucion de uno
		//con el otro

		int indice1 = rand()%data.n;
		int indice2 = rand()%data.n;

		float auxiliar;

		auxiliar = ind->vectorSolucion[indice1];
		ind->vectorSolucion[indice1] = ind->vectorSolucion[indice2];
		ind->vectorSolucion[indice2] = auxiliar;

		ind->valorObjetivo = -1;
	}

	void CPH::operadorCruce(struct datosSolucionCPH *ind1,struct datosSolucionCPH *ind2)
	{
		int auxiliar;
		for(int i = 0; i<data.n; i++)
		{
			if(ind1->vectorSolucion[i] < data.p and ind2->vectorSolucion[i] < data.p and ((double)rand()/RAND_MAX)<=0.5)//es que son clientes
			{
				auxiliar = ind2->vectorSolucion[i];
				ind2->vectorSolucion[i] = ind1->vectorSolucion[i];
				ind1->vectorSolucion[i] = auxiliar;
			}

		}
		ind1->valorObjetivo = -1;
		ind2->valorObjetivo = -1;
	}

	struct datosSolucionCPH ** CPH::seleccionPorTorneo(struct datosSolucionCPH **pob, int tamanoTorneo, int tamanoPoblacion, int numeroSeleccion)
	{
		/*hay que recibir una poblacion de t+2
		ahora, segun el pseudocodigo de las diapositivas, se itera T-1 veces, que será el número
		de elementos que tendrá la población que vamos a crear aqui dentro.

		pelearemos a dos elementos al azar, y se cogera para la nueva población a aquel que haya
		vencido en la lucha y además no se borra ya que puede haber repeticion*/


		//esta es la población donde guardaremos los que ganen el torneo y vuelvan al evolutivo
		struct datosSolucionCPH ** poblacionTorneo = new struct datosSolucionCPH *[numeroSeleccion];
		/*for(int i = 0; i<numeroSeleccion; i++)
		{
			poblacionTorneo[i] = new struct datosSolucionCPH;
			reservarMemoriaSolucion(poblacionTorneo[i])
		}*/

		int indice1, indice2;

		for(int i = 0; i<numeroSeleccion; i++)
		{
			//primero se cogen los indices aleatoriamente de los elementos que vamos a enfrentar
			indice1 = rand()%tamanoPoblacion;
			indice2 = rand()%tamanoPoblacion;

			//ahora se produce el torneo
			if(pob[indice1]->valorObjetivo < pob[indice2]->valorObjetivo)
			{
				poblacionTorneo[i] = pob[indice1];
			}
			else if(pob[indice1]->valorObjetivo > pob[indice2]->valorObjetivo)
			{
				poblacionTorneo[i] = pob[indice2];
			}
			else
			{
				//cuando hay empate da igual cual metamos
				poblacionTorneo[i] = pob[indice1];
			}
		}

		return poblacionTorneo;
	}

	struct datosSolucionCPH ** CPH::seleccionPorRuleta(struct datosSolucionCPH **pob, int tamanoPoblacion, int numeroSeleccion)
	{
		float * vectorAptitudes = new float [tamanoPoblacion], sumTotal = 0.0; //este es el vector p de la diapositiva para las aptitudes

		//primero debemos sacar el total por el que dividiremos la aptitud de cada elemento

		for(int i = 0; i < tamanoPoblacion; i++)
		{
			sumTotal += 1/pob[i]->valorObjetivo;
			vectorAptitudes[i] = sumTotal;
		}

		//ahora sacamos la porcion de ruleta que corresponde a cada elemento

		for(int i = 0; i < tamanoPoblacion; i++)
		{
			vectorAptitudes[i] = vectorAptitudes[i]/sumTotal;
		}

		struct datosSolucionCPH ** pobAux = new struct datosSolucionCPH *[numeroSeleccion];
		for(int i = 0; i < numeroSeleccion; i++)
		{
			pobAux[i] = new struct datosSolucionCPH;
			reservarMemoriaSolucion(pobAux[i]);
		}

		//ahora debemos introducir los numeroSeleccion elementos en la nueva poblacion

		for(int i = 0; i < numeroSeleccion; i++)
		{
			float r = (double)rand()/RAND_MAX;
			for(int j = 0; j < tamanoPoblacion; j++)
			{
				if(vectorAptitudes[j] >= r)
				{
					for(int k = 0; k < data.n; k++)
					{
						pobAux[i]->vectorSolucion[k] = pob[j]->vectorSolucion[k];
					}
					pobAux[i]->valorObjetivo = pob[j]->valorObjetivo;
				}
			}
		}

		return pobAux;
	}

	struct datosSolucionCPH * CPH::mejorSolucionPoblacion(struct datosSolucionCPH **pob, int tamanoPoblacion)
	{
		float solucion=0.0;
		int indice=0;
		float minimo = 32000;

		for(int i=0; i<tamanoPoblacion; i++)
		{
			solucion = pob[i]->valorObjetivo;
			if(solucion < minimo)
			{
				minimo = solucion;
				indice = i;
			}
		}

		return pob[indice];
	}

	struct datosSolucionCPH * CPH::aplicarAlgoritmoGenetico(int tamanoPoblacion, float pCruce, float pMutacion, int nEval, bool estacionario, string archivo)
	{
		int nIteraciones = 0;
		int nGenSinMejorar = 0;
		float r;

		struct datosSolucionCPH ** poblacion = new datosSolucionCPH * [tamanoPoblacion]; //con esto se declara la poblacion todal
		struct datosSolucionCPH * mejor = new datosSolucionCPH;
		reservarMemoriaSolucion(mejor);


		for(int i = 0; i<tamanoPoblacion; i++)
		{
			poblacion[i]=generarSolucionRandom();//hay que generar la solucion random para tener un valor
			poblacion[i]->valorObjetivo = evaluarSolucion(poblacion[i]);//esto es para hacer la evaluacion inicial
			// Incrementar el contador de evaliuaciones
			nIteraciones++;
		}

		int tamano1 = tamanoPoblacion;
		int tamano2 = 2;

		//se declara aqui asi sólo va cambiando su contenido y no hay muchos problemas de memoria

		if(estacionario) //aqui se implementa el AGe
		{
			struct datosSolucionCPH ** seleccionTorneo = new struct datosSolucionCPH * [tamanoPoblacion-1];
			struct datosSolucionCPH ** poblacion3 = new struct datosSolucionCPH *[tamano1+tamano2];

			struct datosSolucionCPH ** poblacion2 = new struct datosSolucionCPH * [2]; //esta es la p' de las diapositivas
			for(int i = 0; i < 2; i++)
			{
				poblacion2[i] = new struct datosSolucionCPH;
				reservarMemoriaSolucion(poblacion2[i]);
			}

			//vamos a abrir el archivo para introducir los datos
			ofstream flujoSalida;
			flujoSalida.open(archivo.c_str(), ios::out);

			//por si falla al abrir el flujo
			if(!flujoSalida)
			{
				cout << "Error al abrir fichero" << endl;
			}

			try
			{
				do
				{

					struct datosSolucionCPH * punteroMejor = mejorSolucionPoblacion(poblacion, tamanoPoblacion);
					for(int i=0; i<data.n; i++)
						mejor->vectorSolucion[i] =punteroMejor->vectorSolucion[i];
					mejor->valorObjetivo = punteroMejor->valorObjetivo;

					poblacion2 = seleccionPorRuleta(poblacion, tamanoPoblacion, 2);

					r = ((double)rand())/RAND_MAX;
					if(r <= pCruce)
					{
						operadorCruce(poblacion2[0], poblacion2[1]);
					}//finsi

					/*el for segundo, que es para marcar para evarluar, tiene que ir fuera de la mutación y con indice
					 * 2 sino da violacion de segmento.
					 */

					for(int i = 0; i<2; i++)
					{
						r = (double)rand()/RAND_MAX;//se crea el alatorio para comparar con pm
						if(r <= pMutacion) //se va a aplicar el operador de mutacion
						{
							operadorMutacion(poblacion2[i]);
						}//finsi
					}//finpara

					nIteraciones = nIteraciones + evaluacion(poblacion2, 2);


					//ahora se realiza la operacion de seleccion uniendo las dos poblaciones y
					//cogiendo una poblacion de T-1 elementos

					//debemos crear una poblacion de T+2 para introducir todos los elementos y esta
					//la pasaremos al torneo

					for(int i=0; i< tamano1; i++)
					   poblacion3[i] = poblacion[i];
					for(int i=tamano1; i< tamano1+tamano2; i++)
					   poblacion3[i] = poblacion2[i-tamano1];


					seleccionTorneo = seleccionPorTorneo(poblacion3, 2, tamano1 + tamano2, tamanoPoblacion - 1);

					for(int j=0; j<tamanoPoblacion-1; j++){
						for(int i=0; i<data.n; i++)
							poblacion[j]->vectorSolucion[i] =seleccionTorneo[j]->vectorSolucion[i];
						poblacion[j]->valorObjetivo = seleccionTorneo[j]->valorObjetivo;

					}

					for(int i=0; i<data.n; i++)
						poblacion[tamanoPoblacion - 1]->vectorSolucion[i] =mejor->vectorSolucion[i];
					poblacion[tamanoPoblacion - 1]->valorObjetivo = mejor->valorObjetivo;

					//hay que actualizar nGenSinMejorar
					int mejora = false;
					for(int i = 0; i<tamanoPoblacion; i++)
					{
						if(mejor->valorObjetivo > poblacion[i]->valorObjetivo)
							mejora=true;
					}
					if(mejora)
						nGenSinMejorar = 0;
					else
						nGenSinMejorar++;
					if(nGenSinMejorar >= 500)
					{
						for(int i = 0; i<tamanoPoblacion; i++)
						{

							poblacion[i]=generarSolucionRandom();
							poblacion[i]->valorObjetivo = evaluarSolucion(poblacion[i]);
							// Incrementar contador de evaluaciones
							nGenSinMejorar = 0;
							nIteraciones++;
						}
					}

					//introducimos los datos en el fichero
					flujoSalida << nIteraciones << "\t"<< mejor->valorObjetivo << endl;

				}while(nIteraciones < 3500 * data.n);
				mejor = mejorSolucionPoblacion(poblacion, tamanoPoblacion);

				flujoSalida << nIteraciones << "\t"<< mejor->valorObjetivo << endl;

				flujoSalida.close();
			}catch(const bad_alloc& e)
			{
				mejor = mejorSolucionPoblacion(poblacion, tamanoPoblacion);
				return mejor;
			}


			for(int i = 0; i<2; i++)
			{
				liberarMemoriaSolucion(poblacion2[i]);
			}
			delete[] poblacion2;
		}
		else //aqui se implementa el AGg
		{
			struct datosSolucionCPH ** poblacion2 = new struct datosSolucionCPH * [tamanoPoblacion-1]; //esta es la p' de las diapositivas
			for(int i = 0; i < tamanoPoblacion-1; i++)
			{
				poblacion2[i] = new struct datosSolucionCPH;
				reservarMemoriaSolucion(poblacion2[i]);
			}

			try
			{
				do
				{

					//primero hay que buscar el mejor
					struct datosSolucionCPH * punteroMejor = mejorSolucionPoblacion(poblacion, tamanoPoblacion);
					for(int i=0; i<data.n; i++)
						mejor->vectorSolucion[i] =punteroMejor->vectorSolucion[i];
					mejor->valorObjetivo = punteroMejor->valorObjetivo;

					//ahora se realiza el operador de seleccion con ruleta de las que cogemos t-1 individuos
					poblacion2 = seleccionPorRuleta(poblacion, tamanoPoblacion, tamanoPoblacion-1);

					//a continuacion el operador de cruce
					for(int i=0; i<tamanoPoblacion-1; i++)
					{
						r = ((double)rand())/RAND_MAX;
						int indice = (i+1+rand()%(tamanoPoblacion-2))%(tamanoPoblacion-1);
						if(r <= pCruce)
						{
							operadorCruce(poblacion2[i], poblacion2[indice]);
						}
					}


					//operador de mutacion
					for(int i=0; i<tamanoPoblacion-1; i++)
					{
						r = ((double)rand())/RAND_MAX;
						if(r <= pMutacion)
							operadorMutacion(poblacion2[i]);
					}

					nIteraciones = nIteraciones + evaluacion(poblacion2, tamanoPoblacion - 1);


					//hay que actualizar nGenSinMejorar
					bool mejora = false;
					for(int i = 0; i<tamanoPoblacion-1; i++)
					{
						if(mejor->valorObjetivo > poblacion2[i]->valorObjetivo)
							mejora=true;
					}
					if(mejora)
						nGenSinMejorar = 0;
					else
						nGenSinMejorar++;
					if(nGenSinMejorar >= 50)
					{
						for(int i = 0; i<tamanoPoblacion-1; i++)
						{

							poblacion2[i]=generarSolucionRandom();
							poblacion2[i]->valorObjetivo = evaluarSolucion(poblacion2[i]);
							// Incrementar contador de evaluaciones
							nGenSinMejorar = 0;
							nIteraciones++;
						}
					}

					//aqui se hace el reemplazo de la poblacion

					for(int i = 0; i<tamanoPoblacion - 1; i++)
					{
						for(int j = 0; j<data.n; j++)
						{
							poblacion[i]->vectorSolucion[j] = poblacion2[i]->vectorSolucion[j];
						}
						poblacion[i]->valorObjetivo = poblacion2[i]->valorObjetivo;
					}


					for(int i=0; i<data.n; i++)
						poblacion[tamanoPoblacion - 1]->vectorSolucion[i] =mejor->vectorSolucion[i];
					poblacion[tamanoPoblacion - 1]->valorObjetivo = mejor->valorObjetivo;

					for(int i = 0; i<tamanoPoblacion-1; i++)
					{
						liberarMemoriaSolucion(poblacion2[i]);
					}

				}while(nIteraciones < 3500 * data.n);
				mejor = mejorSolucionPoblacion(poblacion, tamanoPoblacion);
			}catch(const bad_alloc& e)
			{
				mejor = mejorSolucionPoblacion(poblacion, tamanoPoblacion);
				return mejor;
			}
		}

		return mejor;
	}

	int CPH::evaluacion(struct datosSolucionCPH **pob, int tamanoPoblacion)
	{
		int cuenta = 0;
		for(int i = 0; i < tamanoPoblacion; i++)
		{
			//cerr << "estoy en evaluacion" << pob[i]->valorObjetivo << endl;
			if(pob[i]->valorObjetivo == -1)
			{
				pob[i]->valorObjetivo = evaluarSolucion(pob[i]);
				cuenta ++;
			}
		}
		return cuenta;
	}

}
