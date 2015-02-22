//============================================================================
// Name        : practica1.cpp
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
#include "clases/PerceptronMulticapa.h"

using namespace redneuronal;
using namespace std;

int main(int argc, char **argv) {

    // Procesar la línea de comandos
	bool bflag = false, oflag = false;
	char *tvalue = NULL , *Tvalue = NULL;
	int svalue = 0, fvalue = 0;
	int i=0, l=0, h=0, b=0;
	float eta=0.0, mu=0.0;
	int c=0;

	//Valores por defecto
	i = 1000; //numero de iteraciones
	l = 1; //numero de capas ocultas
	h = 5; //numero de neuronas por capa oculta
	eta = 0.1; //valor del parametro eta
	mu = 0.9; //valor del parametro mu
	b = 0; //este es el valor del sesgo, en caso de que se añada

	while( (c = getopt(argc, argv, "t:T:i:l:h:be:m:f:s")) != -1)
	{
		switch(c)
		{
		case 't':
			tvalue = optarg;
			break;
		case 'T':
			Tvalue = optarg;
			break;
		case 'i':
			i = atoi(optarg);
			break;
		case 'l':
			l = atoi(optarg);
			break;
		case 'h':
			h = atoi(optarg);
			break;
		case 'b':
			b = 1;
			break;
		case 'e':
			eta = atof(optarg);
			break;
		case 'm':
			mu = atof(optarg);
			break;
		case 's':
			svalue = 1;
			break;
		case 'f':
			fvalue = 1;
			break;
		default:
			cout << "No has introducido parametros, se ajustarán por defecto" << endl;
		}
	}

    // Tomar iteraciones, capas y neuronas de la línea de comandos

    // Tomar sesgo, eta y mu de la línea de comandos

    // Tomar fichero de train y de test de la línea de comandos

    // Tomar tipo de error (0 MSE, 1 Entropía cruzada) de la línea de comandos

	PerceptronMulticapa mlp;

	Datos * pDatosTrain = mlp.leerDatos(tvalue);
	Datos * pDatosTest = mlp.leerDatos(Tvalue);

    // Inicializar el vector "topología" (número de neuronas por cada capa, incluyendo la de entrada y la de salida)
    // ...

	int topologia[2+l];

	//neuronas en capa de entrada
	topologia[0] = pDatosTrain->nNumEntradas;

	//neuronas en capas ocultas
	for(int i=1; i<=l; i++)
	{
		topologia[i] = h;
	}

	//neuronas en salida
	topologia[2+l-1]=pDatosTrain->nNumSalidas;

	// Sesgo
	mlp.bSesgo = bflag;

	// Online?
	mlp.bOnline = oflag;

	// Eta
	mlp.dEta = eta;

	// Mu
	mlp.dMu = mu;

    // Inicialización propiamente dicha
	mlp.inicializar(l+2,topologia, svalue);

	cout << "hola" << endl;

	//Inicializacion iteraciones
	int iteraciones = i;

    // Semilla de los números aleatorios
    int semillas[] = {10,20,30,40,50};
    double *errores = new double[5];
    double *erroresTrain = new double[5];
    double *ccrs = new double[5];
    double *ccrsTrain = new double[5];
    for(int i=0; i<5; i++){
    	cout << "**********" << endl;
    	cout << "SEMILLA " << semillas[i] << endl;
    	cout << "**********" << endl;
		srand(semillas[i]);
		mlp.ejecutarAlgoritmo(pDatosTrain,pDatosTest,iteraciones,&(erroresTrain[i]),&(errores[i]),&(ccrsTrain[i]),&(ccrs[i]),fvalue);
		cout << "Finalizamos => CCR de test final: " << ccrs[i] << endl;
    }

    // Calcular media y desviación típica de los errores de Train y de Test
    // ....

    cout << "TERMINADAS LAS CINCO SEMILLAS" << endl;

    float mediaErrorTrain = 0.0;
	float mediaError = 0.0;
	float desviacionTipicaErrorTrain = 0.0;
	float desviacionTipicaError = 0.0;

	float mediaCCRTrain = 0.0;
	float mediaCCR = 0.0;
	float desviacionTipicaCCRTrain = 0.0;
	float desviacionTipicaCCR = 0.0;

	for(int i=0; i<5; i++)
	{
		//media
		mediaErrorTrain += erroresTrain[i];
		mediaError += errores[i];
		mediaCCRTrain += ccrsTrain[i];
		mediaCCR += ccrs[i];
	}

	mediaErrorTrain = mediaErrorTrain / 5;
	mediaError = mediaError / 5;
	mediaCCRTrain = mediaCCRTrain / 5;
	mediaCCR = mediaCCR / 5;

	int i = 0;

	for(i=0; i<5; i++)
	{
		desviacionTipicaErrorTrain += pow((erroresTrain[i] - mediaErrorTrain),2);
		desviacionTipicaError += pow((errores[i] - mediaError),2);
		desviacionTipicaCCRTrain += pow((ccrsTrain[i] - mediaCCRTrain),2);
		desviacionTipicaCCR += pow((ccrs[i] - mediaCCR),2);
	}

	desviacionTipicaErrorTrain = sqrt(desviacionTipicaErrorTrain/i);
	desviacionTipicaError = sqrt(desviacionTipicaError/i);
	desviacionTipicaCCRTrain = sqrt(desviacionTipicaCCRTrain/i);
	desviacionTipicaCCR = sqrt(desviacionTipicaCCR/i);

	cout << "RESUMEN FINAL" << endl;
	cout << "*************" << endl;
    cout << "Error de entrenamiento (Media DT): " << mediaErrorTrain << " " << desviacionTipicaErrorTrain << endl;
    cout << "Error de test (Media DT): " << mediaError << " " << desviacionTipicaError << endl;
    cout << "CCR de entrenamiento (Media DT): " << mediaCCRTrain << " " << desviacionTipicaCCRTrain << endl;
    cout << "CCR de test (Media DT): " << mediaCCR << " " << desviacionTipicaCCR << endl;
	return EXIT_SUCCESS;
}

