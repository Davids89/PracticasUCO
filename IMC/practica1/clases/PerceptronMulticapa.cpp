/*********************************************************************
 * File  : PerceptronMulticapa.cpp
 * Date  : 2014
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <limits>
#include <math.h>
#include <cstring>

#include "PerceptronMulticapa.h"

using namespace redneuronal;
using namespace std;

// ------------------------------
// Obtener un número real aleatorio en el intervalo [Low,High]
double realAleatorio(double Low, double High)
{
	//hay que hacer un aleatorio en un intervalo
	double aleatorio = ((double) rand() / RAND_MAX)*(High-Low) + Low;

	return aleatorio;
}

// ------------------------------
// CONSTRUCTOR: Dar valor por defecto a todos los parámetros
PerceptronMulticapa::PerceptronMulticapa()
{
	bSesgo = true;
		pCapas = NULL;
		dEta = 0.1;
		nNumCapas = 3;
		dMu = 0.9;
}

// Reservar memoria para las estructuras de datos
// nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
// Rellenar vector Capa* pCapas
int PerceptronMulticapa::inicializar(int nl, int npl[]) {

	this->nNumCapas = nl; //aqui asignamos el numero de capas
	this->pCapas = new Capa[nl];

	for(int i = 0; i<nl; i++)
	{
		this->pCapas[i].pNeuronas = new Neurona[npl[i]];
		this->pCapas[i].nNumNeuronas = npl[i];
		//aqui se inicializan las neuronas por capa ya que en cada elemento de npl van las
		//neuronas de cada capa
	}

	for(int i = 0; i<this->nNumCapas; i++)
	{
		if(i == 0)
		{
			for(int j = 0; j<this->pCapas[i].nNumNeuronas; j++)
			{
				this->pCapas[i].pNeuronas[j].w = NULL;
				this->pCapas[i].pNeuronas[j].deltaW = NULL;
				this->pCapas[i].pNeuronas[j].ultimoDeltaW = NULL;
				this->pCapas[i].pNeuronas[j].wCopia = NULL;
			}
		}
		for(int j = 0; j<this->pCapas[i].nNumNeuronas; j++)
		{
			this->pCapas[i].pNeuronas[j].w = new double[npl[i-1]+this->bSesgo];
			this->pCapas[i].pNeuronas[j].deltaW = new double[npl[i-1]+this->bSesgo];
			this->pCapas[i].pNeuronas[j].ultimoDeltaW = new double[npl[i-1]+this->bSesgo];
			this->pCapas[i].pNeuronas[j].wCopia = new double[npl[i-1]+this->bSesgo];
		}
	}

	//los bSesgos son para sumar la posicion del sesgo, en caso de que la haya, y para poder colocarlo al final
	//de todas las neuronas

	return 1;
}


// ------------------------------
// DESTRUCTOR: liberar memoria
PerceptronMulticapa::~PerceptronMulticapa() {
	liberarMemoria();
}


// ------------------------------
// Liberar memoria para las estructuras de datos
void PerceptronMulticapa::liberarMemoria() {

	for(int capas = 0; capas < this->nNumCapas; capas++)
	{
		for(int neuronas = 0; neuronas < this->pCapas[capas].nNumNeuronas; neuronas++)
		{
			delete [] this->pCapas[capas].pNeuronas[neuronas].deltaW;
			delete [] this->pCapas[capas].pNeuronas[neuronas].ultimoDeltaW;
			delete [] this->pCapas[capas].pNeuronas[neuronas].w;
			delete [] this->pCapas[capas].pNeuronas[neuronas].wCopia;
		}
		delete [] this->pCapas[capas].pNeuronas;
	}
	delete [] this->pCapas;
}

// ------------------------------
// Rellenar todos los pesos (w) aleatoriamente entre -1 y 1
void PerceptronMulticapa::pesosAleatorios() {

	//tenemos que entrar en la capa, y dentro de ella en sus neuronas y
	//poner los pesos aleatorios entre -1 y 1

	double valor = 0.0;

	for(int i = 1; i<this->nNumCapas; i++)
	{
		for(int j = 0; j<this->pCapas[i].nNumNeuronas; j++)
		{
			for(int k = 0; k<this->pCapas[i-1].nNumNeuronas+this->bSesgo; k++)
			{
				valor = realAleatorio(-1,1);
				this->pCapas[i].pNeuronas[j].w[k]=valor;
			}
		}
	}
}

// ------------------------------
// Alimentar las neuronas de entrada de la red con un patrón pasado como argumento
void PerceptronMulticapa::alimentarEntradas(double* input) {

	//Aqui recomemos las neuronas de la capa de entrada y le vamos
	//asignando el valor de entrada que viene por parametro

	for(int neurona = 0; neurona < this->pCapas[0].nNumNeuronas; neurona++)
	{
		this->pCapas[0].pNeuronas[neurona].x = input[neurona];
	}

}

// ------------------------------
// Recoger los valores predichos por la red (out de la capa de salida) y almacenarlos en el vector pasado como argumento
void PerceptronMulticapa::recogerSalidas(double* output) {

	//Recorremos la capa de salida y vamos asignando el valor de la neurona
	//con el valor correspondiente del vector del parametro

	for(int neurona = 0; neurona < this->pCapas[this->nNumCapas-1].nNumNeuronas; neurona++)
	{
		output[neurona] = this->pCapas[this->nNumCapas-1].pNeuronas[neurona].x;
	}

}

// ------------------------------
// Hacer una copia de todos los pesos (copiar w en copiaW)
void PerceptronMulticapa::copiarPesos() {

	for(int capa = 1; capa < this->nNumCapas; capa++)
	{
		for(int neurona = 0; neurona < this->pCapas[capa].nNumNeuronas; neurona++)
		{
			for(int i = 0; i < this->pCapas[capa-1].nNumNeuronas+this->bSesgo; i++)
			{
				this->pCapas[capa].pNeuronas[neurona].wCopia[i] = this->pCapas[capa].pNeuronas[neurona].w[i];
			}
		}
	}

}

// ------------------------------
// Restaurar una copia de todos los pesos (copiar copiaW en w)
void PerceptronMulticapa::restaurarPesos() {

	for(int capa = 1; capa < nNumCapas; capa++)
		{
			for(int neurona = 0; neurona < pCapas[capa].nNumNeuronas; neurona++)
			{
				for(int x = 0; x < pCapas[capa-1].nNumNeuronas+this->bSesgo; x++)
				{
					pCapas[capa].pNeuronas[neurona].w[x] = pCapas[capa].pNeuronas[neurona].wCopia[x];
				}
			}
		}

}

// ------------------------------
// Calcular y propagar las salidas de las neuronas, desde la primera capa hasta la última
void PerceptronMulticapa::propagarEntradas() {

	double net = 0.0;

	for(int capa = 1; capa < nNumCapas; capa++)
	{
		for(int neurona = 0; neurona < pCapas[capa].nNumNeuronas; neurona++)
		{

			//net << - w0 - Sum w*out^h-1
			for(int i = 0; i < pCapas[capa-1].nNumNeuronas; i++)
			{
				net -= pCapas[capa-1].pNeuronas[neurona].x * pCapas[capa].pNeuronas[neurona].w[i];
			}

			if(bSesgo)
			{
				net -= pCapas[capa].pNeuronas[neurona].w[pCapas[capa-1].nNumNeuronas];
			}

			//out << o(net)
			pCapas[capa].pNeuronas[neurona].x = 1 / (1 + exp(net));
		}
	}

}

// ------------------------------
// Calcular el error de salida (MSE) del out de la capa de salida con respecto a un vector objetivo y devolverlo
double PerceptronMulticapa::calcularErrorSalida(double* target) {

	double error = 0.0;

	int salidas = 0;

	//La ecuacion del error es Sum(di - oi)²
	for(salidas = 0; salidas < this->pCapas[this->nNumCapas-1].nNumNeuronas; salidas++)
	{
		error += pow((target[salidas] - this->pCapas[this->nNumCapas-1].pNeuronas[salidas].x),2);
	}

	//ahora dividimos por el numero de salidas

	return error / pCapas[nNumCapas-1].nNumNeuronas;
}


// ------------------------------
// Retropropagar el error de salida con respecto, desde la última capa hasta la primera
void PerceptronMulticapa::retropropagarError(double* objetivo) {

	//Para cada salida
	//djh = - (dj - outjh)*outjh * (1 - outjh)
		for(int j = 0; j < pCapas[nNumCapas-1].nNumNeuronas; j++)
		{
			pCapas[nNumCapas-1].pNeuronas[j].dX = - (objetivo[j] - pCapas[nNumCapas-1].pNeuronas[j].x) * pCapas[nNumCapas-1].pNeuronas[j].x * (1 - pCapas[nNumCapas-1].pNeuronas[j].x);
		}

		//Para cada etapa
		for(int h = nNumCapas-2; h > 0; h--)
		{
			//Para cada neurona de cada etapa
			for(int j = 0; j < pCapas[h].nNumNeuronas; j++)
			{
				//Sum(w*deltaW)
				double sumatorio = 0;
				for(int x = 0; x < pCapas[h+1].nNumNeuronas; x++)
				{
					sumatorio += (pCapas[h+1].pNeuronas[x].w[j] * pCapas[h+1].pNeuronas[x].dX);
				}
				pCapas[h].pNeuronas[j].dX = sumatorio * pCapas[h].pNeuronas[j].x * (1 - pCapas[h].pNeuronas[j].x);
			}
		}

}

// ------------------------------
// Acumular los cambios producidos por un patrón en deltaW
void PerceptronMulticapa::acumularCambio() {

	for(int capa = 1; capa < this->nNumCapas; capa++)
	{
		for(int neurona = 0; neurona < this->pCapas[capa].nNumNeuronas; neurona++)
		{
			for(int i = 0; i< this->pCapas[capa - 1].nNumNeuronas; i++)
			{
				this->pCapas[capa].pNeuronas[neurona].ultimoDeltaW[i] = this->pCapas[capa].pNeuronas[neurona].deltaW[i];
				this->pCapas[capa].pNeuronas[neurona].deltaW[i] += this->pCapas[capa].pNeuronas[neurona].dX * this->pCapas[capa-1].pNeuronas[neurona].x;
			}
			if(this->bSesgo)
			{
				this->pCapas[capa].pNeuronas[neurona].ultimoDeltaW[pCapas[capa-1].nNumNeuronas] = this->pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas];
				this->pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas] += pCapas[capa].pNeuronas[neurona].dX;
			}
		}
	}

}

// ------------------------------
// Actualizar los pesos de la red, desde la primera capa hasta la última
void PerceptronMulticapa::ajustarPesos() {

	for(int capa = 1; capa < this->nNumCapas; capa++)
	{
		for(int neurona = 0; neurona < this->pCapas[capa].nNumNeuronas; neurona++)
		{
			for(int i = 0; i<this->pCapas[capa - 1].nNumNeuronas; i++)
			{
				pCapas[capa].pNeuronas[neurona].w[i]-= dEta*pCapas[capa].pNeuronas[neurona].deltaW[i] + dMu * (dEta * pCapas[capa].pNeuronas[neurona].ultimoDeltaW[i]);

			}

			if(bSesgo)
			{
				pCapas[capa].pNeuronas[neurona].w[pCapas[capa-1].nNumNeuronas]-= dEta*pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas] + dMu * (dEta * pCapas[capa].pNeuronas[neurona].ultimoDeltaW[pCapas[capa-1].nNumNeuronas]);

			}
		}
	}
}

// ------------------------------
// Imprimir la red, es decir, todas las matrices de pesos
void PerceptronMulticapa::imprimirRed() {

	for(int capa = 1; capa < this->nNumCapas; capa++)
	{
		cout << "Capa: " << capa << endl;
		cout << "---------------" << endl;

		for(int neurona = 0; neurona < this->pCapas[capa].nNumNeuronas; neurona++)
		{
			for(int i = 0; i < this->pCapas[capa-1].nNumNeuronas+this->bSesgo; i++)
			{
					cout << this->pCapas[capa].pNeuronas[neurona].w[i] << " ";
			}
			cout << "Salida: " << this->pCapas[capa].pNeuronas[neurona].x << endl;
			cout << endl;
		}
		cout << endl;
	}

}

// ------------------------------
// Simular la red: propragar las entradas hacia delante, computar el error, retropropagar el error y ajustar los pesos
// entrada es el vector de entradas del patrón, objetivo es el vector de salidas deseadas del patrón, y salida deberá
// ser utilizado para almacenar la predicción realizada por la red
void PerceptronMulticapa::simularRedOnline(double* entrada, double* salida, double* objetivo) {

	//Tenemos que implementar el para que hay en la diapositiva

	//Primero tenemos que poner todos los valores w a 0
	for(int capa = 1; capa < this->nNumCapas; capa++)
	{
		for(int neurona = 0; neurona < this->pCapas[capa].nNumNeuronas; neurona++)
		{
			//como tenemos que coger valores de w tenemos que iterar la capa anterior
			for(int i = 0; i < this->pCapas[capa-1].nNumNeuronas; i++)
			{
				pCapas[capa].pNeuronas[neurona].ultimoDeltaW[i]=pCapas[capa].pNeuronas[neurona].deltaW[i];
				pCapas[capa].pNeuronas[neurona].deltaW[i]=0;
			}

			if(bSesgo)
			{
				pCapas[capa].pNeuronas[neurona].ultimoDeltaW[pCapas[capa-1].nNumNeuronas]=pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas];
				pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas]=0;
			}
		}
	}

	alimentarEntradas(entrada);
	propagarEntradas();
	retropropagarError(objetivo);
	acumularCambio();
	ajustarPesos();
	recogerSalidas(salida);
}

// ------------------------------
// Leer una matriz de datos a partir de un nombre de fichero y devolverla
Datos* PerceptronMulticapa::leerDatos(const char *archivo) {

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
			for(int j = data->nNumEntradas; j < data->nNumEntradas + data->nNumSalidas; j++)
			{
				int indiceSalida = 1;
				fichero >> valor;
				data->salidas[i][indiceSalida] = valor;
				indiceSalida++;
			}
		}

		//Se cierra el fichero
	    fichero.close();

	    //Se devuelven las matrices
	    return data;

}

// ------------------------------
// Entrenar la red on-line para un determinado fichero de datos
void PerceptronMulticapa::entrenarOnline(Datos* pDatosTrain) {
	int i;
	for(i=0; i<pDatosTrain->nNumPatrones; i++){
		double* prediccion = new double[pDatosTrain->nNumSalidas];
		simularRedOnline(pDatosTrain->entradas[i], prediccion, pDatosTrain->salidas[i]);
		delete[] prediccion;
	}
}

// Probar la red con un conjunto de datos y devolver el error MSE cometido
// tanto para entrenamiento como para generalizacion
double PerceptronMulticapa::test(Datos* pDatosTest) {
	int i;
	double dAvgTestError = 0;
	for(i=0; i<pDatosTest->nNumPatrones; i++){
		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas();
		dAvgTestError += calcularErrorSalida(pDatosTest->salidas[i]);
	}
	dAvgTestError /= pDatosTest->nNumPatrones;

	return dAvgTestError;
}

// Ejecutar el algoritmo un número de iteraciones
void PerceptronMulticapa::ejecutarAlgoritmoOnline(Datos * pDatosTrain, Datos * pDatosTest, int maxiter, double *errorTrain, double *errorTest)
{
	ofstream fichero("datos.txt"); //fichero para almacenar los datos de salida

	int countTrain = 0;

	// Inicialización de pesos
	pesosAleatorios();

	double minTrainError = 0;
	int numSinMejorar;
	double testError = 0;

	// Aprendizaje del algoritmo
	do {
		entrenarOnline(pDatosTrain);
		double trainError = test(pDatosTrain);
		if(countTrain==0 || fabs(trainError - minTrainError) > 0.00001){
			minTrainError = trainError;
			copiarPesos();
			numSinMejorar = 0;
		}
		else{
			numSinMejorar++;
		}

		if(numSinMejorar==50){
			restaurarPesos();
			countTrain = maxiter;
		}

		countTrain++;

		cout << "Iteración " << countTrain << "\t Error de entrenamiento: " << trainError << endl;
		fichero << countTrain << "\t " << trainError << endl;

	} while ( countTrain<maxiter );

	cout << "PESOS DE LA RED" << endl;
	cout << "===============" << endl;
	imprimirRed();

	cout << "Salida Esperada Vs Salida Obtenida (test)" << endl;
	cout << "=========================================" << endl;
	for(int i=0; i<pDatosTest->nNumPatrones; i++){
		double* prediccion = new double[pDatosTest->nNumSalidas];

		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas();
		recogerSalidas(prediccion);
		for(int j=0; j<pDatosTest->nNumSalidas; j++)
			cout << pDatosTest->salidas[i][j] << " -- " << prediccion[j];
		cout << endl;
		delete[] prediccion;

	}

	testError = test(pDatosTest);
	*errorTest=testError;
	*errorTrain=minTrainError;

	fichero.close();

}
