//============================================================================
// Name        : practica3.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
// Description : Hello World in C, Ansi-style
//============================================================================

//============================================================================
// Name        : practica3.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <ctime>    // Para cojer la hora time()
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <string.h>
#include <math.h>
#include "clases/RedRBF.h"

using namespace redneuronal;
using namespace std;

void calcularNeuronas(Datos *Datum, bool Softmax, double Porcentaje )
{
	Datum->nCentroides = 0;
	double aux = 0;
	if(Softmax) //Problema de clasificación
	{
		Datum->matrixIndices.resize(Datum->nNumSalidas);
		Datum->vectorIndices.resize(Datum->nNumSalidas);

		for(int i = 0; i < Datum->nNumPatrones; i++)
		{
			for(int j = 0; j < Datum->nNumSalidas; j++ )
			{
				if(Datum->salidas[i][j] == 1)
				{
					Datum->matrixIndices[j].push_back(i);
					j = Datum->nNumSalidas;
				}
			}
		}

		for(int i = 0; i < Datum->nNumSalidas; i++)
		{
			aux = (Datum->matrixIndices[i].size() * Porcentaje) / 100.0;
			if(aux <= 0.01 && aux < 1)
			{
				Datum->vectorIndices[i]++;
				Datum->nCentroides++;
			}
			else
			{
				if(aux >= 1)
				{
					Datum->vectorIndices[i] += (int) aux;
					Datum->nCentroides += (int) aux;
				}
				else
				{
					cout << "Error: Patrones insuficientes de la clase " << i << " para entrenar"<< endl;
					exit(-1);
				}
			}
		}

	}
	else // Problema de regresion
	{
		Datum->nCentroides = (int)(Datum->nNumPatrones * Porcentaje) / 100;
	}
}

int main(int argc, char **argv){
	// Procesar la línea de comandos
	char *tvalue = NULL, *Tvalue = NULL;
	int hvalue = 50, c;
	float evalue = 0.01;
	bool svalue = false;

	opterr = 0;
	while ((c = getopt (argc, argv, "t:T:h:e:s")) != -1)
	{
		switch (c)
		{
			case 't': //fichero de train
				tvalue = optarg;
				break;
			case 'T': //fichero de test
				Tvalue = optarg;
				break;
			case 'h': //numero de neuronas capa oculta
				hvalue = atoi(optarg);
				break;
			case 'e': //parametro eta
				evalue = atof(optarg);
				break;
			case 's': //si se usa sigmoide o no
				svalue = true;
				break;
			default:
				exit (-1);
		}
	}

    RedRBF rbf;

    //Aqui cogemos los ficheros de train y test

    Datos * pDatosTrain = rbf.leerDatos(tvalue);
    Datos * pDatosTest = rbf.leerDatos(Tvalue);

    //Calculamos las neuronas en capa oculta
    calcularNeuronas(pDatosTrain,svalue,hvalue);

	// Inicializar el vector "topología" (número de neuronas por cada capa, incluyendo la de entrada y la de salida)
	//Vector de tamaño 3
	int topologia[3];
	//Capa 0 es la capa de entrada
	topologia[0] = pDatosTrain->nNumEntradas;
	//Capa oculta
	topologia[1] = pDatosTrain->nCentroides;
	//Capa salida
	topologia[2] = pDatosTrain->nNumSalidas;

    // Softmax
    rbf.bSoftmax = svalue;

    // Valor de eta
    rbf.dEta = evalue;

    rbf.inicializar(3,topologia);

    // Semilla de los números aleatorios
    int semillas[] = {10,20,30,40,50};
    double *erroresTest = new double[5];
    double *erroresTrain = new double[5];
    double *ccrsTest = new double[5];
    double *ccrsTrain = new double[5];
    for(int i=0; i<1; i++){ //se ha puesto 1 al final del bucle para la practica 4 de IMC
    	cout << "**********" << endl;
    	cout << "SEMILLA " << semillas[i] << endl;
    	cout << "**********" << endl;
		srand(semillas[i]);
		rbf.ejecutarAlgoritmo(pDatosTrain,pDatosTest,&(erroresTrain[i]),&(erroresTest[i]),&(ccrsTrain[i]),&(ccrsTest[i]),(int)svalue);
		if(svalue)
			cout << "Finalizamos => CCR de test final: " << ccrsTest[i] << endl;
		else
			cout << "Finalizamos => Error de test final: " << erroresTest[i] << endl;
    }

    // Calcular media y desviación típica de los errores de Train y de Test
	double mediaErrorTrain = 0.0;
	double desviacionTipicaErrorTrain = 0.0;
	double mediaErrorTest = 0.0;
	double desviacionTipicaErrorTest = 0.0;
	double mediaCCRTrain = 0.0;
	double desviacionTipicaCCRTrain = 0.0;
	double mediaCCRTest = 0.0;
	double desviacionTipicaCCRTest = 0.0;

	int n = 0;
	for(n = 0; n < 1; n++) //se ha puesto 1 al final del bucle para la practica 4 de IMC
	{
		mediaErrorTrain += erroresTrain[n];
		mediaErrorTest += erroresTest[n];
		mediaCCRTrain += ccrsTrain[n];
		mediaCCRTest += ccrsTest[n];
	}
	mediaErrorTrain /= n;
	mediaErrorTest /= n;
	mediaCCRTrain /= n;
	mediaCCRTest /= n;

	for(n = 0; n < 1; n++) //se ha puesto 1 al final del bucle para la practica 4 de IMC
	{
		desviacionTipicaErrorTrain += pow(erroresTrain[n] - mediaErrorTrain,2);
		desviacionTipicaErrorTest += pow(erroresTest[n] - mediaErrorTest,2);
		desviacionTipicaCCRTrain += pow(ccrsTrain[n] - mediaCCRTrain,2);
		desviacionTipicaCCRTest += pow(ccrsTest[n] - mediaCCRTest,2);
	}

	desviacionTipicaErrorTrain = sqrt(desviacionTipicaErrorTrain/n);
	desviacionTipicaErrorTest = sqrt(desviacionTipicaErrorTest/n);
	desviacionTipicaCCRTrain = sqrt(desviacionTipicaCCRTrain/n);
	desviacionTipicaCCRTest = sqrt(desviacionTipicaCCRTest/n);

    cout << "TERMINADAS LAS CINCO SEMILLAS" << endl;

    cout << "RESUMEN FINAL" << endl;
    cout << "*************" << endl;
    cout << "Error de entrenamiento (Media +- DT): " << mediaErrorTrain << " +- " << desviacionTipicaErrorTrain << endl;
    cout << "Error de test (Media +- DT): " << mediaErrorTest << " " << desviacionTipicaErrorTest << endl;
    cout << "CCR de entrenamiento (Media +- DT): " << mediaCCRTrain << " +- " << desviacionTipicaCCRTrain << endl;
    cout << "CCR de test (Media +- DT): " << mediaCCRTest << " +- " << desviacionTipicaCCRTest << endl;

    return EXIT_SUCCESS;
}


