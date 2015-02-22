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

int main(int argc, char **argv) {
    // Procesar la línea de comandos

    // Tomar neuronas de la línea de comandos

    // Tomar eta de la línea de comandos

    // Tomar fichero de train y de test de la línea de comandos

    // Tomar softmax de la línea de comandos


    RedRBF rbf;

    // Datos de entrenamiento y test
    Datos * pDatosTrain = rbf.leerDatos(tvalue);
    Datos * pDatosTest = rbf.leerDatos(Tvalue);

    // Inicializar el vector "topología" (número de neuronas por cada capa, incluyendo la de entrada y la de salida)
    // ...

    // Softmax
    rbf.bSoftmax = sflag;

    // Valor de eta
    rbf.dEta = eta;

    rbf.inicializar(1+2,topologia);


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
		rbf.ejecutarAlgoritmo(pDatosTrain,pDatosTest,&(erroresTrain[i]),&(errores[i]),&(ccrsTrain[i]),&(ccrs[i]),(int)sflag);
		if(sflag)
			cout << "Finalizamos => CCR de test final: " << ccrs[i] << endl;
		else
			cout << "Finalizamos => Error de test final: " << errores[i] << endl;
    }

    // Calcular media y desviación típica de los errores de Train y de Test
    // ....

    cout << "TERMINADAS LAS CINCO SEMILLAS" << endl;

    cout << "RESUMEN FINAL" << endl;
    cout << "*************" << endl;
    cout << "Error de entrenamiento (Media +- DT): " << mediaErrorTrain << " +- " << desviacionTipicaErrorTrain << endl;
    cout << "Error de test (Media +- DT): " << mediaError << " " << desviacionTipicaError << endl;
    cout << "CCR de entrenamiento (Media +- DT): " << mediaCCRTrain << " +- " << desviacionTipicaCCRTrain << endl;
    cout << "CCR de test (Media +- DT): " << mediaCCR << " +- " << desviacionTipicaCCR << endl;

    return EXIT_SUCCESS;
}

