/*********************************************************************
 * File  : RedRBF.h
 * Date  : 2014
 *********************************************************************/


#ifndef _REDRBF_H_
#define _REDRBF_H_

// Librerías auxiliares

#include "kmeans/kmeans.hpp"     //Fichero de cabecera para poder utilizar la libreria kmeans
#include "maxent-3.0/maxent.h"   //Fichero de cabecera para poder utilizar la libreria maxent-3.0
#include "matrix/matrix.h"       //Fichero de cabecera para poder utilizar la libreria matrix

namespace redneuronal{

// Estructuras sugeridas
// ---------------------
struct Neurona {
	double  x;            /* Salida producida por la neurona (out_j^h)*/
	double* w;            /* Vector de pesos de entrada (w_{ji}^h)*/
};

struct Capa {
	int     nNumNeuronas;   /* Número de neuronas de la capa*/
	Neurona* pNeuronas;     /* Vector con las neuronas de la capa*/
};

struct Datos {
	int nNumEntradas;     /* Número de entradas */
	int nNumSalidas;      /* Número de salidas */
	int nNumPatrones;     /* Número de patrones */
	double** entradas;    /* Matriz con las entradas del problema */
	double** salidas;     /* Matriz con las salidas del problema */
};

class RedRBF {
private:
	int    nNumCapas;     /* Número de capas total en la red (siempre va a ser 3) */
	Capa* pCapas;         /* Vector con cada una de las capas (siempre tendrá 3 elementos) */

	// Liberar memoria para las estructuras de datos
	void liberarMemoria();

	// Alimentar las neuronas de entrada de la red con un patrón pasado como argumento
	void alimentarEntradas(double* entrada);

	// Recoger los valores predichos por la red (out de la capa de salida) y almacenarlos en el vector pasado como argumento
	void recogerSalidas(double* salida);

	// Calcular y propagar las salidas de las neuronas, para una capa específica (argumento capa)
	// propagarEntradas(1) propagará la primera capa oculta, propagarEntradas(2) propagará la capa de salida
	void propagarEntradas(int capa);

	// Calcular el error de salida con respecto a un vector objetivo y devolverlo
	// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
	double calcularErrorSalida(double* objetivo, int funcionError);

	// Imprimir la red, es decir, todas las matrices de pesos
	void imprimirRed();

	// Función para aplicar clustering, aplica el algoritmo de clustering sobre la matriz de entradas de pDatos, con objeto
	// de obtener los nNumClusters centroides maś representativos. Devuelve los centroides en una matriz
	double ** clusteringEntradas(Datos *pDatos, int nNumClusters);

	// Funciones necesarias para interactuar con la librería kmeans, transforman una matriz de doubles a un vector,
	// donde todos los elementos están seguidos (y viceversa)
	double * matrizAVector(double **X, int nNumFilas, int nNumColumnas);
	double ** vectorAMatriz(double *X, int nNumFilas, int nNumColumnas);

	// Función para aplicar regresión logística, aplica el algoritmo de regresión logística sobre la matriz de entradas "entradas" (R en las
	// diapositivas y teniendo en cuenta las salidas definidas en "salidas" (Y en las diapositivas). El número de filas y columnas de las matrices
	// viene definido en las variables nNumPatrones y nNumEntradas (para "entradas") y nNumPatrones y nNumSalidas (para "salidas")
	// Devuelve los parámetros optimizados en una matriz (Beta en las diapositivas)
	double ** regresionLogistica(double **entradas, double **salidas, int nNumPatrones, int nNumEntradas, int nNumSalidas);

	// Funciones necesarias para interactuar con la librería maxent-3.0, transforman una matriz de entradas y una matriz
	// de salida al objeto ME_Model, que nos permitirá ejecutar la regresión logística. La segunda función devuelve la
	// matriz de coeficientes del modelo (donde las filas son el número de clases y la columnas el número de variables utilizadas)
	ME_Model modeloRegLog(double **entradas, double **salidas, int nNumPatrones, int nNumEntradas, int nNumSalidas);
	double ** matrixCoeficientesModeloRegLog(ME_Model model, int nNumEntradas, int nNumSalidas);

	// Función para aplicar la pseudoinversa en problemas de regresión, aplica el algoritmo de la pseudoinversa sobre la matriz de entradas "entradas" (R en las
	// diapositivas y teniendo en cuenta las salidas definidas en "salidas" (Y en las diapositivas). El número de filas y columnas de las matrices
	// viene definido en las variables nNumPatrones y nNumEntradas (para "entradas") y nNumPatrones y nNumSalidas (para "salidas")
	// Devuelve los parámetros optimizados en una matriz (Beta en las diapositivas)
	double ** pseudoinversa(double **entradas, double **salidas, int nNumPatrones, int nNumEntradas, int nNumSalidas);

	// Funciones necesarias para interactuar con la librería matrix, transforman una matriz de doubles a un objeto tipo Matrix
	// al que luego podremos aplicar operaciones de álgebra lineal
	Matrix matrizDoubleAObjeto(double **X, int nNumFilas, int nNumColumnas);
	double ** matrizObjetoADouble(Matrix X, int nNumFilas, int nNumColumnas);


public:
	bool   bSoftmax;      // ¿Es clasificación?
	double dEta;          // Parámetro de regularización

	// Constructor
	RedRBF();

	// Destructor
	~RedRBF();

	// Reservar memoria para las estructuras de datos
    // nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
	int inicializar(int nl, int npl[]);

	// Probar la red con un conjunto de datos y devolver el error cometido
	// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
	double test(Datos* pDatosTest, int funcionError);

	// Probar la red con un conjunto de datos y devolver el error CCR cometido
	// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
	double testClassification(Datos* pDatosTest);

	// Leer una matriz de datos a partir de un nombre de fichero y devolverla
	Datos* leerDatos(const char *archivo);

	// Ejecutar el algoritmo de entrenamiento utilizando pDatosTrain
	// Una vez terminado, probar como funciona la red en pDatosTest
	// Tanto el error MSE o Entropia de entrenamiento como el de test debe calcularse y almacenarse en errorTrain y errorTest
	// Lo mismo para el CCR si está activa la softmax
	// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
	void ejecutarAlgoritmo(Datos * pDatosTrain, Datos * pDatosTest, double *errorTrain, double *errorTest, double *ccrTrain, double *ccrTest, int funcionError);
};

};

#endif
