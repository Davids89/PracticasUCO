//============================================================================
// Name        : practica1.cpp
// Author      : Pedro A. Gutiérrez
// Version     :
// Copyright   : Universidad de Córdoba
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
    bool bflag = false;
    char *tvalue = NULL , *Tvalue = NULL;
    int i=0, l=0, h=0, b=0;
    float eta=0.0, mu=0.0;
    int c;
    // ....

    // Tomar iteraciones, capas y neuronas de la línea de comandos

    // Tomar sesgo, eta y mu de la línea de comandos

    // Tomar fichero de train y de test de la línea de comandos

    PerceptronMulticapa mlp;

    //Valores por defecto
    i = 1000; //numero de iteraciones
    l = 1; //numero de capas ocultas
    h = 5; //numero de neuronas por capa oculta
    eta = 0.1; //valor del parametro eta
    mu = 0.9; //valor del parametro mu
    b = 1; //este es el valor del sesgo, en caso de que se añada

    while( (c = getopt(argc, argv, "t:T:i:l:h:be:m:")) != -1)
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
        		break;
        	case 'e':
        		eta = atof(optarg);
        		break;
        	case 'm':
        		mu = atof(optarg);
        		break;
        	default:
        		cout << "No has introducido parametros, se ajustarán por defecto" << endl;
        	}
        }

    Datos * pDatosTrain = mlp.leerDatos(tvalue);
    Datos * pDatosTest = mlp.leerDatos(Tvalue);

    cout << "Se han leido los datos" << endl;

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


    //nNumEntradas es el numero de entradas, h determina las neuronas en capa oculta
    //l es el numero de capas ocultas, y nNumSalida es el numero de salidas

    cout << "Se declara topologia bien" << endl;

    // Sesgo
    mlp.bSesgo = bflag; //si hay sesgo siempre va a ser 1

    // Eta
    mlp.dEta = eta;

    // Mu
    mlp.dMu = mu;

    //Inicializacion iteraciones
    int iteraciones = 1000 * pDatosTrain->nNumPatrones;

    // Inicialización propiamente dicha
    mlp.inicializar(l+2,topologia);

    // Semilla de los números aleatorios
    int semillas[] = {10,20,30,40,50};
    double *erroresTest = new double[5]; //uno por cada semilla
    double *erroresTrain = new double[5];
    for(int i=0; i<5; i++){
        srand(semillas[i]);
    	cout << "**********" << endl;
    	cout << "SEMILLA " << semillas[i] << endl;
    	cout << "**********" << endl;
        mlp.ejecutarAlgoritmoOnline(pDatosTrain,pDatosTest,iteraciones,&(erroresTrain[i]),&(erroresTest[i]));
		cout << "Finalizamos => Error de test final: " << erroresTest[i] << endl;
    }

    cout << "TERMINADAS LAS CINCO SEMILLAS" << endl;

    // Calcular media y desviación típica de los errores de Train y de Test
    // ....

    float mediaErrorTrain = 0.0;
    float mediaErrorTest = 0.0;
    float desviacionTipicaErrorTrain = 0.0;
    float desviacionTipicaErrorTest = 0.0;

    for(int i=0; i<5; i++)
    {
    	//media
    	mediaErrorTrain += erroresTrain[i];
    	mediaErrorTest += erroresTest[i];
    }

    mediaErrorTrain = mediaErrorTrain / 5;
    mediaErrorTest = mediaErrorTest / 5;

    cout << "RESUMEN FINAL" << endl;
    cout << "*************" << endl;
    cout << "Error de entrenamiento (Media +- DT): " << mediaErrorTrain << " +- " << desviacionTipicaErrorTrain << endl;
    cout << "Error de test (Media +- DT):          " << mediaErrorTest << " +- " << desviacionTipicaErrorTest << endl;
    return EXIT_SUCCESS;
}

