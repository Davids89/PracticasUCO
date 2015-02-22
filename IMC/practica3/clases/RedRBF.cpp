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

	//Abre el fichero en modo lectura
	ifstream fichero;
	fichero.open(archivo);

	//Estructura de datos
	Datos * data = (Datos *) malloc(sizeof(Datos));

	//Lee numEntradsa, numSalidas y numPatrones
	fichero >> data->nNumEntradas >> data->nNumSalidas >> data->nNumPatrones;

	//Reserva de memoria de la matriz
	data->entradas = new double *[data->nNumPatrones];
	data->salidas = new double *[data->nNumPatrones];
	for (int i = 0; i < data->nNumPatrones; i++)
	{
		data->entradas[i] = new double[data->nNumEntradas];
		data->salidas[i] = new double [data->nNumSalidas];
	}

	//Lee los valores
	double valor;
	for(int i = 0; i < data->nNumPatrones; i++)
	{
		//n valores de entrada
		for(int j = 0; j < data->nNumEntradas; j++)
		{
			fichero >> valor;
			data->entradas[i][j] = valor;
		}
		//k valores de salida
		for(int j = 0; j < data->nNumSalidas; j++)
		{
			fichero >> valor;
			data->salidas[i][j] = valor;
		}
	}

	//Se cierra el fichero
	fichero.close();

	//Se devuelven las matrices
	return data;
}


// ------------------------------
// Reservar memoria para las estructuras de datos
// nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
int redneuronal::RedRBF::inicializar(int nl, int npl[]) {

	//Numero de capas
	this->nNumCapas = nl;

	//Reservamos memoria para las capas
	this->pCapas = new Capa[nl];

	//Reservamos la memoria de cada capa
	for(int i = 0; i < nl; i++)
	{
		this->pCapas[i].pNeuronas = new Neurona [npl[i]];
		this->pCapas[i].nNumNeuronas = npl[i];
	}

	//Reservamos el vector w que vamos a usar en las neuronas
	for(int i = 0; i < this->nNumCapas; i++)
	{
		if(i == 0)
		{
			for(int j = 0; j < this->pCapas[i].nNumNeuronas; j++)
			{
				this->pCapas[i].pNeuronas[j].w = NULL;
			}
		}
		else
		{
			for(int j = 0; j < this->pCapas[i].nNumNeuronas; j++)
			{
				this->pCapas[i].pNeuronas[j].w = new double [npl[i-1]+1];
			}
		}
	}

	return 1;
}

// ------------------------------
// Liberar memoria para las estructuras de datos
void redneuronal::RedRBF::liberarMemoria() {

	for(int h = 0; h < this->nNumCapas-1; h++)
	{
		for(int j = 0; j < this->pCapas[h].nNumNeuronas; j++)
		{
				delete [] this->pCapas[h].pNeuronas[j].w;
		}
		delete [] this->pCapas[h].pNeuronas;
	}
	delete [] this->pCapas;

}

// ------------------------------
// Alimentar las neuronas de entrada de la red con un patrón pasado como argumento
void redneuronal::RedRBF::alimentarEntradas(double* entrada) {

	//Aqui recomemos las neuronas de la capa de entrada y le vamos
	//asignando el valor de entrada que viene por parametro

	for(int neurona = 0; neurona < this->pCapas[0].nNumNeuronas; neurona++)
	{
		this->pCapas[0].pNeuronas[neurona].x = entrada[neurona];
	}

}

// ------------------------------
// Recoger los valores predichos por la red (out de la capa de salida) y almacenarlos en el vector pasado como argumento
void redneuronal::RedRBF::recogerSalidas(double* salida) {

	//Recorremos la capa de salida y vamos asignando el valor de la neurona
	//con el valor correspondiente del vector del parametro

	for(int neurona = 0; neurona < this->pCapas[this->nNumCapas-1].nNumNeuronas; neurona++)
	{
		salida[neurona] = this->pCapas[this->nNumCapas-1].pNeuronas[neurona].x;
	}

}

// ------------------------------
// Calcular y propagar las salidas de las neuronas, para una capa específica (argumento capa)
// propagarEntradas(1) propagará la primera capa oculta, propagarEntradas(2) propagará la capa de salida
void redneuronal::RedRBF::propagarEntradas(int capa) {

	double net = 0;
	if(capa == 1) //Propagacion primera capa a capa oculta
	{
		for(int i = 0; i < this->pCapas[1].nNumNeuronas; i++)
		{
			net = 0;
			for(int j = 0; j < this->pCapas[0].nNumNeuronas; j++)
			{
				net += pow(this->pCapas[0].pNeuronas[j].x - this->pCapas[1].pNeuronas[i].w[j],2); //Net
			}
			net /= -(2 * pow(this->pCapas[1].pNeuronas[i].w[this->pCapas[0].nNumNeuronas],2)); //-Net/2*radio²
			this->pCapas[1].pNeuronas[i].x = exp(net); // exp(-Net/2*radio²)
		}
	}
	else if(capa == 2) //Propagacion capa oculta a capa de salida
	{
		for(int i = 0; i < this->pCapas[2].nNumNeuronas; i++)
		{
			net = 0;
			for(int j = 0; j < this->pCapas[1].nNumNeuronas; j++)
			{
				net += this->pCapas[1].pNeuronas[j].x * this->pCapas[2].pNeuronas[i].w[j]; //Net
			}
			net += this->pCapas[2].pNeuronas[i].w[this->pCapas[1].nNumNeuronas]; //Sesgo
			this->pCapas[2].pNeuronas[i].x = net;
		}

		if(this->bSoftmax)// Problema de clasificacion
		{
			net = 0;
			for (int i = 0; i < this->pCapas[2].nNumNeuronas; i++)
			{
				this->pCapas[2].pNeuronas[i].x = exp(this->pCapas[2].pNeuronas[i].x);
				net += this->pCapas[2].pNeuronas[i].x;
			}
			for (int i = 0; i < this->pCapas[2].nNumNeuronas; i++)
			{
				this->pCapas[2].pNeuronas[i].x /= net;
			}
		}
	}

}

// ------------------------------
// Calcular el error de salida con respecto a un vector objetivo y devolverlo
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double redneuronal::RedRBF::calcularErrorSalida(double* objetivo, int funcionError) {

		double aux = 0.0;

		if(funcionError == 0)//MSE
		{
			for(int n = 0; n < this->pCapas[nNumCapas-1].nNumNeuronas; n++)
			{
				aux += pow(objetivo[n] - this->pCapas[nNumCapas-1].pNeuronas[n].x, 2);
			}
		}
		else //EntropiaCruzada
		{
			for(int n = 0; n < this->pCapas[nNumCapas-1].nNumNeuronas; n++)
			{
				aux += (objetivo[n] * log(this->pCapas[nNumCapas-1].pNeuronas[n].x));
			}
		}

		return aux / this->pCapas[nNumCapas-1].nNumNeuronas;
}

// ------------------------------
// Imprimir la red, es decir, todas las matrices de pesos
void redneuronal::RedRBF::imprimirRed() {

		for(int h = 1; h < this->nNumCapas; h++)
		{
			cout << "Capa " << h << endl;
			cout << "--------" << endl;

			for(int j = 0; j < this->pCapas[h].nNumNeuronas; j++)
			{
				for(int i = 0; i < this->pCapas[h-1].nNumNeuronas; i++)
				{
					cout << this->pCapas[h].pNeuronas[j].w[i] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
}

// Constructor
redneuronal::RedRBF::RedRBF()
{

}

// ------------------------------
// Destructor
redneuronal::RedRBF::~RedRBF() {
	liberarMemoria();
}


// ------------------------------
// Probar la red con un conjunto de datos y devolver el error cometido
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double redneuronal::RedRBF::test(Datos* pDatosTest, int funcionError) {

	double testError = 0.0;

	for( int i = 0; i < pDatosTest->nNumPatrones; i++)
	{
		//debemos introducir unas entradas y propagar las entradas
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas(1);
		propagarEntradas(2);

		testError += calcularErrorSalida(pDatosTest->salidas[i],funcionError);
	}

	if(funcionError) //estamos en entropia cruzada
		return testError /= -(pDatosTest->nNumPatrones);
	else //aqui estamos en MSE
		return testError /= pDatosTest->nNumPatrones;

}

// ------------------------------
// Probar la red con un conjunto de datos y devolver el error CCR cometido
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double redneuronal::RedRBF::testClassification(Datos* pDatosTest) {

	mIndices matrizConfusion(pDatosTest->nNumSalidas,vIndices(pDatosTest->nNumSalidas));
	//la matriz confusion es una matriz de vectores de enteros compuesta por
	//nNumSalidas vectores
	//con vectores de enteros de nNumSalidas elementos
	//es decir, es una matriz cuadrada

	double ccrAux = 0.0;

	for( int i = 0; i < pDatosTest->nNumPatrones; i++)
	{
		//primero ante todo hay que introducir unas entradas y propagarlas
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas(1);
		propagarEntradas(2);

		//Calculo de CCR
		int yDeseada = 0;
		int yObtenida = 0;
		double maximoSalida = 0.0;
		double maximoTarget = 0.0;

		for( int j = 0; j < this->pCapas[this->nNumCapas-1].nNumNeuronas; j++)
		{
			if( pDatosTest->salidas[i][j] > maximoTarget)
			{
				maximoTarget = pDatosTest->salidas[i][j];
				yDeseada = j;
			}

			if( this->pCapas[this->nNumCapas-1].pNeuronas[j].x > maximoSalida)
			{
				maximoSalida = this->pCapas[this->nNumCapas-1].pNeuronas[j].x;
				yObtenida = j;
			}
		}

		//ahora comprobamos si hay acierto
		if(yDeseada == yObtenida)
		{
			ccrAux++;
		}

		//ahora sumamos uno en el elemento correspondiente de la matriz de confusion
		matrizConfusion[yObtenida][yDeseada]++;
	}

	//Mostramos la matriz de confusion
	cout << "Matriz de confusion" << endl;
	cout << "=====================" << endl;
	for(int j = 0; j < pDatosTest->nNumSalidas; j++)
	{
		for(int k = 0; k < pDatosTest->nNumSalidas; k++)
		{
			if(k < pDatosTest->nNumSalidas-1)
				cout << matrizConfusion[j][k] << " - " ;
			else
				cout << matrizConfusion[j][k] << endl ;
		}
	}
	cout << endl;
	return 100 * (ccrAux/pDatosTest->nNumPatrones);

}

double **RedRBF::matrizDistancias(double **Datos, int fil, int col)
{
	double ** aux = new double * [fil];

	for( int i = 0; i < fil; i++)
	{
		aux[i] = new double [fil];
	}

	//inicializamos a cero
	for( int i = 0; i < fil; i++)
	{
		aux[i][i] = 0;
	}

	double acu = 0.0;
	for(int i = 0; i < fil; i++)
	{
		for(int j = i+1; j < fil; j++)
		{
			acu = 0;
			for(int k = 0; k < col;k++)
			{
				acu += pow(Datos[i][k]-Datos[j][k],2);
			}
			aux[i][j] = sqrt(acu);
			aux[j][i] = sqrt(acu);
		}
	}

	return aux;
}

// ------------------------------
// Ejecutar el algoritmo de entrenamiento utilizando pDatosTrain
// Una vez terminado, probar como funciona la red en pDatosTest
// Tanto el error MSE o Entropia de entrenamiento como el de test debe calcularse y almacenarse en errorTrain y errorTest
// Lo mismo para el CCR si está activa la softmax
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
void redneuronal::RedRBF::ejecutarAlgoritmo(Datos* pDatosTrain, Datos* pDatosTest, double* errorTrain, double* errorTest, double* ccrTrain, double* ccrTest, int funcionError) {

	//cerr << "Numero neuronas " << this->pCapas[1].nNumNeuronas << endl;

	double ** centroides = clusteringEntradas(pDatosTrain,pCapas[1].nNumNeuronas);

	// Introducimos los centroides en pCapas[1].pNeuronas[i].w[j]
	for( int i = 0; i < this->pCapas[1].nNumNeuronas; i++)
	{
		for( int j = 0; j < this->pCapas[0].nNumNeuronas; j++)
		{
			this->pCapas[1].pNeuronas[i].w[j] = centroides[i][j];
		}
	}

	cout << "Calculando radios de las RBF..." << endl;
	// Calculamos los radios

	double ** distanciaCentroides = matrizDistancias(centroides,pDatosTrain->nCentroides,pDatosTrain->nNumEntradas);
	double sumaDistancias = 0.0;

	//Hay que recorrer los centroides y acumular las distancias
	for( int i = 0; i < pDatosTrain->nCentroides; i++)
	{
		sumaDistancias=0.0;
		for( int j = 0; j < pDatosTrain->nCentroides; j++)
		{
			sumaDistancias += distanciaCentroides[i][j];
		}
		//ahora usamos la heuristica de la mitad de la media de las distancias hacia el resto de centros
		this->pCapas[1].pNeuronas[i].w[pCapas[0].nNumNeuronas] = sumaDistancias/(2*(this->pCapas[1].nNumNeuronas-1));
	}

	// Obtener la matriz de respuestas (R) de la red

	double ** R = new double *[pDatosTrain->nNumPatrones];

	for( int i = 0; i < pDatosTrain->nNumPatrones; i++)
	{
		R[i] = new double [this->pCapas[1].nNumNeuronas+1];
		alimentarEntradas(pDatosTrain->entradas[i]);
		//debemos recoger la salida de la capa 1
		propagarEntradas(1);
		for( int j = 0; j < this->pCapas[1].nNumNeuronas+1; j++)
		{
			R[i][j] = this->pCapas[1].pNeuronas[j].x;
		}
		R[i][pCapas[1].nNumNeuronas] = 1;//para la columna del sesgo
	}

	// Obtener los pesos de la última capa
	double ** coeficientesSalida;

	if(bSoftmax)
		coeficientesSalida = regresionLogistica(R,pDatosTrain->salidas,pDatosTrain->nNumPatrones,pCapas[1].nNumNeuronas+1,pDatosTrain->nNumSalidas);
	else
		coeficientesSalida = pseudoinversa(R,pDatosTrain->salidas,pDatosTrain->nNumPatrones,pCapas[1].nNumNeuronas+1,pDatosTrain->nNumSalidas);

	// Introducimos los coeficientesSalida en pCapas[2].pNeuronas[i].w[j]

	for(int i = 0; i < pCapas[2].nNumNeuronas; i++)
	{
		for(int j = 0; j < pCapas[1].nNumNeuronas+1; j++)
		{
			pCapas[2].pNeuronas[i].w[j] = coeficientesSalida[i][j];
		}
	}

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

double **RedRBF::centroidesInicio(Datos *pDatos)
{
	double ** matrizAux = new double * [pDatos->nCentroides];

	//Vector numeros aleatorios
	vector<int> pSeleccionados;

	if(bSoftmax)//Problema de clasificacion
	{
		int aux = 0;
		for(unsigned int i = 0; i < pDatos->vectorIndices.size(); i++)
		{
			pSeleccionados = vectorAleatoriosSinRepeticion(pDatos->matrixIndices[i].size(),0,pDatos->vectorIndices[i]);
			for(unsigned int j = 0; j < pSeleccionados.size(); j++)
			{
				matrizAux[aux] = pDatos->entradas[pSeleccionados[j]];
				aux++;
			}
		}
	}
	else //Problema de regresion. Seleccion aleatoria de n1 patrones
	{
		pSeleccionados = vectorAleatoriosSinRepeticion(pDatos->nNumPatrones-1,0,pDatos->nCentroides);
		for(unsigned int i = 0; i < pSeleccionados.size(); i++)
		{
			matrizAux[i] = pDatos->entradas[pSeleccionados[i]];
		}
	}

	return matrizAux;
}

vector<int> RedRBF::vectorAleatoriosSinRepeticion(int high, int low, int n)
{
	int total = high-low+1;

	//Vector de numeros aleatorios
	vector<int> numerosPorElegir(total);
	vector<int> numerosElegidos(n);

	for(int i = 0; i < total; i++)
	{
		numerosPorElegir[i] = low+i;
	}

	for(int i = 0; i < n; i++)
	{
		int numeroElegido = rand()%(total-i);
		numerosElegidos[i]=numerosPorElegir[numeroElegido];
		numerosPorElegir[numeroElegido]=numerosPorElegir[total-i-1];
	}
	return numerosElegidos;
}

// ------------------------------
// Función para aplicar clustering, aplica el algoritmo de clustering sobre la matriz de entradas de pDatos, con objeto
// de obtener los nNumClusters centroides maś representativos. Devuelve los centroides en una matriz
double ** RedRBF::clusteringEntradas(Datos *pDatos, int nNumClusters){

	int kmeans = 200; //iteraciones del kmeans
	int i = 0; //hay que pasarlo a la funcion
	int * cluster = new int[pDatos->nNumPatrones];
	int * poblacion = new int [nNumClusters];
	double * distancias=new double [nNumClusters];
	double ** centroides = centroidesInicio(pDatos);
	double *centroidesIniciales = matrizAVector(centroides,nNumClusters,pDatos->nNumEntradas);

	cerr << "Numero clusters " << nNumClusters << endl;

	kmeans_01(pDatos->nNumEntradas,pDatos->nNumPatrones,nNumClusters,kmeans,i,matrizAVector(pDatos->entradas,pDatos->nNumPatrones,pDatos->nNumEntradas),cluster,centroidesIniciales,poblacion,distancias);

	return vectorAMatriz(centroidesIniciales,nNumClusters,pDatos->nNumEntradas);
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

		ME_Model model = modeloRegLog(entradas,salidas,nNumPatrones,nNumEntradas,nNumSalidas);
		model.use_l2_regularizer(dEta);
		model.train();
		return	matrixCoeficientesModeloRegLog(model,nNumEntradas,nNumSalidas);

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

	Matrix matriz1 = matrizDoubleAObjeto(entradas,nNumPatrones,nNumEntradas);//R
	Matrix matriz2 = matrizDoubleAObjeto(salidas,nNumPatrones,nNumSalidas);//Y
	Matrix pseudoInversa = power((transpose(matriz1)*matriz1),-1) * transpose(matriz1);//(Rt * R)-¹ * Rt
	Matrix beta = pseudoInversa * matriz2;//B = R+ * Y
	beta.t(); //hacer la traspuesta
	return matrizObjetoADouble(beta,beta.dim1(),beta.dim2());
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
			coeficientes[i][j] = X(i,j);

	}
	return coeficientes;

}
