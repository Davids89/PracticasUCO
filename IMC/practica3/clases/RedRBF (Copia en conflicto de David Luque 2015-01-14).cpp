/*********************************************************************
 * File  : RedRBF.cpp
 * Date  : 2014
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <limits>
#include <math.h>
#include <stdio.h>


#include "RedRBF.h"

using namespace redneuronal;
using namespace std;

// ------------------------------
// Leer una matriz de datos a partir de un nombre de fichero y devolverla
Datos* RedRBF::leerDatos(const char *archivo) {

}


// ------------------------------
// Reservar memoria para las estructuras de datos
// nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
int redneuronal::RedRBF::inicializar(int nl, int npl[]) {

}

// ------------------------------
// Liberar memoria para las estructuras de datos
void redneuronal::RedRBF::liberarMemoria() {

}

// ------------------------------
// Alimentar las neuronas de entrada de la red con un patrón pasado como argumento
void redneuronal::RedRBF::alimentarEntradas(double* entrada) {

}

// ------------------------------
// Recoger los valores predichos por la red (out de la capa de salida) y almacenarlos en el vector pasado como argumento
void redneuronal::RedRBF::recogerSalidas(double* salida) {

}

// ------------------------------
// Calcular y propagar las salidas de las neuronas, para una capa específica (argumento capa)
// propagarEntradas(1) propagará la primera capa oculta, propagarEntradas(2) propagará la capa de salida
void redneuronal::RedRBF::propagarEntradas(int capa) {



}

// ------------------------------
// Calcular el error de salida con respecto a un vector objetivo y devolverlo
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double redneuronal::RedRBF::calcularErrorSalida(double* objetivo, int funcionError) {

}

// ------------------------------
// Imprimir la red, es decir, todas las matrices de pesos
void redneuronal::RedRBF::imprimirRed() {

}

// ------------------------------
// Destructor
redneuronal::RedRBF::~RedRBF() {

}


// ------------------------------
// Probar la red con un conjunto de datos y devolver el error cometido
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double redneuronal::RedRBF::test(Datos* pDatosTest, int funcionError) {

}

// ------------------------------
// Probar la red con un conjunto de datos y devolver el error CCR cometido
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double redneuronal::RedRBF::testClassification(Datos* pDatosTest) {

}

// ------------------------------
// Ejecutar el algoritmo de entrenamiento utilizando pDatosTrain
// Una vez terminado, probar como funciona la red en pDatosTest
// Tanto el error MSE o Entropia de entrenamiento como el de test debe calcularse y almacenarse en errorTrain y errorTest
// Lo mismo para el CCR si está activa la softmax
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
void redneuronal::RedRBF::ejecutarAlgoritmo(Datos* pDatosTrain,
		Datos* pDatosTest, double* errorTrain, double* errorTest,
		double* ccrTrain, double* ccrTest, int funcionError) {

	double ** centroides = clusteringEntradas(pDatosTrain,pCapas[1].nNumNeuronas);

	// Introducimos los centroides en pCapas[1].pNeuronas[i].w[j]


	cout << "Calculando radios de las RBF..." << endl;
	// Calculamos los radios

	// Obtener la matriz de respuestas (R) de la red

	// Obtener los pesos de la última capa
	double ** coeficientesSalida;

	if(bSoftmax)
		coeficientesSalida = regresionLogistica(respuestas,pDatosTrain->salidas,pDatosTrain->nNumPatrones,pCapas[1].nNumNeuronas+1,pDatosTrain->nNumSalidas);
	else
		coeficientesSalida = pseudoinversa(respuestas,pDatosTrain->salidas,pDatosTrain->nNumPatrones,pCapas[1].nNumNeuronas+1,pDatosTrain->nNumSalidas);

	// Introducimos los coeficientesSalida en pCapas[2].pNeuronas[i].w[j]

	cout << "PESOS DE LA RED" << endl;
	cout << "===============" << endl;

	imprimirRed();

	//getchar();
	cout << "Salida Esperada Vs Salida Obtenida (test)" << endl;
	cout << "=========================================" << endl;
	for(int i=0; i<pDatosTest->nNumPatrones; i++){
		double* prediccion = new double[pDatosTest->nNumSalidas];

		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		for(int j=1; j<nNumCapas; j++)
			propagarEntradas(j);
		recogerSalidas(prediccion);
		for(int j=0; j<pDatosTest->nNumSalidas; j++)
			cout << pDatosTest->salidas[i][j] << " -- " << prediccion[j] << " \\\\ " ;
		cout << endl;
		delete[] prediccion;

	}

	*errorTest=test(pDatosTest,funcionError);
	*errorTrain=test(pDatosTrain,funcionError);

	if(bSoftmax){
		*ccrTest = testClassification(pDatosTest);
		*ccrTrain = testClassification(pDatosTrain);
	}
	else{
		*ccrTest = *ccrTrain = 0;
	}

}

// ------------------------------
// Función para aplicar clustering, aplica el algoritmo de clustering sobre la matriz de entradas de pDatos, con objeto
// de obtener los nNumClusters centroides maś representativos. Devuelve los centroides en una matriz
double ** RedRBF::clusteringEntradas(Datos *pDatos, int nNumClusters){

}

// ------------------------------
// Funciones necesarias para interactuar con la librería kmeans, transforman una matriz de doubles a un vector,
// donde todos los elementos están seguidos (y viceversa)
double * RedRBF::matrizAVector(double **X, int nNumFilas, int nNumColumnas){
	double *resultado = new double[nNumFilas*nNumColumnas];
	for(int i=0; i<nNumFilas; i++)
		for(int j=0; j<nNumColumnas; j++)
			resultado[i*nNumColumnas+j] = X[i][j];
	return resultado;
}

// ------------------------------
// Funciones necesarias para interactuar con la librería kmeans, transforman una matriz de doubles a un vector,
// donde todos los elementos están seguidos (y viceversa)
double ** RedRBF::vectorAMatriz(double *X, int nNumFilas, int nNumColumnas){
	double **resultado = new double* [nNumFilas];
	for(int i=0; i<nNumFilas; i++){
		resultado[i] = new double[nNumColumnas];
		for(int j=0; j<nNumColumnas; j++)
			resultado[i][j]=X[i*nNumColumnas+j];
	}
	return resultado;
}

// ------------------------------
// Función para aplicar regresión logística, aplica el algoritmo de regresión logística sobre la matriz de entradas "entradas" (R en las
// diapositivas y teniendo en cuenta las salidas definidas en "salidas" (Y en las diapositivas). El número de filas y columnas de las matrices
// viene definido en las variables nNumPatrones y nNumEntradas (para "entradas") y nNumPatrones y nNumSalidas (para "salidas")
// Devuelve los parámetros optimizados en una matriz (Beta en las diapositivas)
double ** RedRBF::regresionLogistica(double **entradas, double **salidas, int nNumPatrones, int nNumEntradas, int nNumSalidas){


}


// ------------------------------
// Funciones necesarias para interactuar con la librería maxent-3.0, transforman una matriz de entradas y una matriz
// de salida al objeto ME_Model, que nos permitirá ejecutar la regresión logística. La segunda función devuelve la
// matriz de coeficientes del modelo (donde las filas son el número de clases y la columnas el número de variables utilizadas)
ME_Model RedRBF::modeloRegLog(double **entradas, double **salidas, int nNumPatrones, int nNumEntradas, int nNumSalidas){
	ME_Model model;

	for(int i=0; i<nNumPatrones; i++){

		char *etiqueta = new char[50];
		for(int j=0; j<nNumSalidas; j++)
			if(salidas[i][j]==1)
				sprintf(etiqueta,"%d",j);

		ME_Sample sample(etiqueta);

		for(int j=0; j<nNumEntradas; j++){
			char *variable = new char[50];
			sprintf(variable,"%d",j);
			sample.add_feature(variable,entradas[i][j]);
		}
		model.add_training_sample(sample);
	}
	return model;
}

// ------------------------------
// Funciones necesarias para interactuar con la librería maxent-3.0, transforman una matriz de entradas y una matriz
// de salida al objeto ME_Model, que nos permitirá ejecutar la regresión logística. La segunda función devuelve la
// matriz de coeficientes del modelo (donde las filas son el número de clases y la columnas el número de variables utilizadas)
double ** RedRBF::matrixCoeficientesModeloRegLog(ME_Model model, int nNumEntradas, int nNumSalidas){
	double ** resultado = new double*[nNumSalidas];
	for(int i=0; i<nNumSalidas; i++)
		resultado[i] = new double[nNumEntradas];
	// Obtener una lista de pares de listas de dos strings y un double
	list< pair< pair<string, string>, double > > fl;
	model.get_features(fl);
	for (list< pair< pair<string, string>, double> >::const_iterator i = fl.begin(); i != fl.end(); i++)
		resultado[atoi(i->first.first.c_str())][atoi(i->first.second.c_str())] = i->second;
	return resultado;
}


// ------------------------------
// Función para aplicar la pseudoinversa en problemas de regresión, aplica el algoritmo de la pseudoinversa sobre la matriz de entradas "entradas" (R en las
// diapositivas y teniendo en cuenta las salidas definidas en "salidas" (Y en las diapositivas). El número de filas y columnas de las matrices
// viene definido en las variables nNumPatrones y nNumEntradas (para "entradas") y nNumPatrones y nNumSalidas (para "salidas")
// Devuelve los parámetros optimizados en una matriz (Beta en las diapositivas)
double ** RedRBF::pseudoinversa(double **entradas, double **salidas, int nNumPatrones, int nNumEntradas, int nNumSalidas){

}

// ------------------------------
// Funciones necesarias para interactuar con la librería matrix, transforman una matriz de doubles a un objeto tipo Matrix
// al que luego podremos aplicar operaciones de álgebra lineal
Matrix RedRBF::matrizDoubleAObjeto(double **X, int nNumFilas, int nNumColumnas){
	Matrix XRes(nNumFilas,nNumColumnas,0);
	for(int i=0; i<nNumFilas; i++)
		for(int j=0; j<nNumColumnas; j++)
			XRes(i,j) = X[i][j];
	return XRes;

}

// ------------------------------
// Funciones necesarias para interactuar con la librería matrix, transforman una matriz de doubles a un objeto tipo Matrix
// al que luego podremos aplicar operaciones de álgebra lineal
double ** RedRBF::matrizObjetoADouble(Matrix X, int nNumFilas, int nNumColumnas){

	double ** coeficientes = new double*[nNumFilas];
	for(int i=0; i<nNumFilas; i++){
		coeficientes[i]  = new double[nNumColumnas];
		for(int j=0; j<nNumColumnas; j++)
			coeficientes[i][j] = X(j,i);

	}
	return coeficientes;

}
