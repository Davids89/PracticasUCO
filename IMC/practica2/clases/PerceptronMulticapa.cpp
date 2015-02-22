/*********************************************************************
 * File  : PerceptronMulticapa.cpp
 * Date  : 2013
 *********************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // Para establecer la semilla srand() y generar números aleatorios rand()
#include <limits>
#include <math.h>
#include <vector>

#include "PerceptronMulticapa.h"

using namespace redneuronal;
using namespace std;
typedef vector<int> vectorMC;
typedef vector<vectorMC> matriz;

// ------------------------------
// Obtener un número entero aleatorio en el intervalo [Low,High]
int enteroAleatorio(int Low, int High) {
	return (int) Low + (rand()%(High+1-Low));
}

// ------------------------------
// Obtener un número real aleatorio en el intervalo [Low,High]
double realAleatorio(double Low, double High) {

	//hay que hacer un aleatorio en un intervalo
	double aleatorio = ((double) rand() / RAND_MAX)*(High-Low) + Low;

	return aleatorio;

}

// ------------------------------
// CONSTRUCTOR: Dar valor por defecto a todos los parámetros
PerceptronMulticapa::PerceptronMulticapa() {

	bSesgo = false;
	pCapas = NULL;
	dEta = 0.1;
	nNumCapas = 3;
	dMu = 0.9;
	bOnline = false;

}

// ------------------------------
// Reservar memoria para las estructuras de datos
// nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
int PerceptronMulticapa::inicializar(int nl, int npl[],bool bSigmoideCapaSalida) {

	this->nNumCapas = nl; //aqui asignamos el numero de capas
	this->pCapas = new Capa[nl];

	int k;

	if(bSigmoideCapaSalida)
	{
		pCapas[nNumCapas-1].tipo = 1;
	}

	for(k = 0; k<nl; k++)
	{
		this->pCapas[k].pNeuronas = new Neurona[npl[k]];
		this->pCapas[k].nNumNeuronas = npl[k];
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
// Destructor
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
// Rellenar todos los pesos aleatoriamente
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

	double net=0,sumatorioNet=0;

	for(int capa=1;capa<nNumCapas;capa++)//para cada capa
	{
		for(int neurona=0;neurona<pCapas[capa].nNumNeuronas;neurona++)//para cada neurona de la capa
		{
			net=0.0;//inicilizamos el net a 0
			for(int i=0;i<pCapas[capa-1].nNumNeuronas;i++)//por cada neurona de la capa anterior
			{

				if(capa==nNumCapas-1 && this->pCapas[this->nNumCapas-1].tipo == 1)//si estoy en ultima capa y uso softmax
				{
					net+= pCapas[capa-1].pNeuronas[i].x * pCapas[capa].pNeuronas[neurona].w[i];
				}
				else //sino
				{
					net-= pCapas[capa-1].pNeuronas[i].x * pCapas[capa].pNeuronas[neurona].w[i] ;
				}
			}

			if(bSesgo)
			{
				if(capa==nNumCapas-1 && this->pCapas[this->nNumCapas-1].tipo == 1)
				  {
					net += pCapas[capa].pNeuronas[neurona].w[pCapas[capa-1].nNumNeuronas];
				  }
				else
				{
					net -= pCapas[capa].pNeuronas[neurona].w[pCapas[capa-1].nNumNeuronas];
				}
			}

			if(capa==nNumCapas-1 && this->pCapas[this->nNumCapas-1].tipo == 1)
			{
				pCapas[capa].pNeuronas[neurona].x=net;
			}
			else
			{
				pCapas[capa].pNeuronas[neurona].x=1/(1+exp(net));
			}
		}
	}

	if(this->pCapas[this->nNumCapas-1].tipo == 1)
	{
		for(int neurona=0;neurona<pCapas[nNumCapas-1].nNumNeuronas;neurona++)
		{
			pCapas[nNumCapas-1].pNeuronas[neurona].x= exp(pCapas[nNumCapas-1].pNeuronas[neurona].x);
			sumatorioNet+=pCapas[nNumCapas-1].pNeuronas[neurona].x;
		}

		for(int neurona=0;neurona<pCapas[nNumCapas-1].nNumNeuronas;neurona++)
		{
			pCapas[nNumCapas-1].pNeuronas[neurona].x/= sumatorioNet;
		}
	}

}

// ------------------------------
// Calcular el error de salida con respecto a un vector objetivo y devolverlo
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double PerceptronMulticapa::calcularErrorSalida(double* target, int funcionError) {

	if(!funcionError)//esta parte calcula MSE
	{
		double error = 0.0;

		//La ecuacion del error es Sum(di - oi)²
		for(int salidas = 0; salidas < this->pCapas[this->nNumCapas-1].nNumNeuronas; salidas++)
		{
			error += pow((target[salidas] - this->pCapas[this->nNumCapas-1].pNeuronas[salidas].x),2);
		}

		//ahora dividimos por el numero de salidas v

		return error / pCapas[nNumCapas-1].nNumNeuronas;
	}
	else //Entropia cruzada
	{
		double error = 0.0;
		//aqui va la entropia cruzada
		for(int salidas = 0; salidas < this->pCapas[this->nNumCapas-1].nNumNeuronas; salidas++)
		{
			error += target[salidas] * log(this->pCapas[this->nNumCapas-1].pNeuronas[salidas].x);
		}
		return error / pCapas[nNumCapas-1].nNumNeuronas;
	}

}


// ------------------------------
// Retropropagar el error de salida con respecto a un vector objetivo, desde la última capa hasta la primera
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
void PerceptronMulticapa::retropropagarError(double* objetivo, int funcionError) {

	//IMPORTANTE
	/*
	 *
	 * PARA EVITAR ERRORES MUY GRANDES, CUANDO HAGAMOS LAS DERIVADAS DEL ERROR RESPECTO A CADA PATRÓN, HAY QUE MULTIPLICAR POR 1/P, NUMERO PATRONES,
	 * PARA EVITAR QUE SE VAYA MUCHO EL ERROR
	 *
	 */

	int nUltimaCapa = this->pCapas[nNumCapas-1].nNumNeuronas;
	double sum=0.0, sumatorioNet=0.0, entropia1=0.0, entropia2=0.0;

	if(this->pCapas[this->nNumCapas-1].tipo == 1) //si estoy usando softmax
	{
		for(int i=0;i<nUltimaCapa;i++)
		{
			sumatorioNet=0;
			for(int j=0;j<nUltimaCapa;j++)
			{

				if(funcionError)//aqui calculo el primer multiplicando de la entropia cruzada
					entropia1=objetivo[j]/this->pCapas[nNumCapas-1].pNeuronas[j].x;
				else
					entropia1=objetivo[j]-this->pCapas[nNumCapas-1].pNeuronas[j].x;

				if(j==i)//aqui va el segundo multiplicando de la entropia cruzada
					entropia2=this->pCapas[nNumCapas-1].pNeuronas[i].x*(1-this->pCapas[nNumCapas-1].pNeuronas[j].x);
				else
					entropia2=this->pCapas[nNumCapas-1].pNeuronas[i].x*(0-this->pCapas[nNumCapas-1].pNeuronas[j].x);

				sumatorioNet-=entropia1*entropia2;//ahora multiplicamos para tener delta
			}
			this->pCapas[nNumCapas-1].pNeuronas[i].dX=sumatorioNet; //lo insertamos en delta
		}
	}
	else //aqui voy a usar entropia cruzada para la sigmoide o el MSE para la sigmoide
	{
			for(int i=0;i<nUltimaCapa;i++)
			{
				if(funcionError)
					this->pCapas[nNumCapas-1].pNeuronas[i].dX= -(objetivo[i]/this->pCapas[nNumCapas-1].pNeuronas[i].x) * this->pCapas[nNumCapas-1].pNeuronas[i].x * (1-this->pCapas[nNumCapas-1].pNeuronas[i].x);
				else
					this->pCapas[nNumCapas-1].pNeuronas[i].dX= -(objetivo[i]-this->pCapas[nNumCapas-1].pNeuronas[i].x) * this->pCapas[nNumCapas-1].pNeuronas[i].x * (1-this->pCapas[nNumCapas-1].pNeuronas[i].x);
			}
	}

	//Vamos retropropagando el error a capas anteriores
	for(int i=this->nNumCapas-2;i>=1;i--)
	{
		for(int j=0;j<this->pCapas[i].nNumNeuronas;j++)
		{
			sum=0.0;
			for(int k=0;k<this->pCapas[i+1].nNumNeuronas;k++)
			{
				sum+= this->pCapas[i+1].pNeuronas[k].w[j] * this->pCapas[i+1].pNeuronas[k].dX;
			}
			this->pCapas[i].pNeuronas[j].dX= sum * this->pCapas[i].pNeuronas[j].x * (1-this->pCapas[i].pNeuronas[j].x);
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
				this->pCapas[capa].pNeuronas[neurona].deltaW[i] += this->pCapas[capa].pNeuronas[neurona].dX * this->pCapas[capa-1].pNeuronas[i].x;
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

	for(int h = 1; h < nNumCapas; h++)
		{
			for(int j = 0; j < pCapas[h].nNumNeuronas; j++)
			{
				for(int i = 0; i < pCapas[h-1].nNumNeuronas; i++)
				{
					pCapas[h].pNeuronas[j].w[i] -= dEta * pCapas[h].pNeuronas[j].deltaW[i] - dMu * (dEta * pCapas[h].pNeuronas[j].ultimoDeltaW[i]);
				}
				if(bSesgo)
				{
					pCapas[h].pNeuronas[j].w[pCapas[h-1].nNumNeuronas] -= dEta * pCapas[h].pNeuronas[j].deltaW[pCapas[h-1].nNumNeuronas] - dMu * (dEta * pCapas[h].pNeuronas[j].ultimoDeltaW[pCapas[h-1].nNumNeuronas]);
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
// El paso de ajustar pesos solo deberá hacerse si el algoritmo es on-line
// Si no lo es, el ajuste de pesos hay que hacerlo en la función "entrenar"
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
void PerceptronMulticapa::simularRed(double* entrada, double* salida, double* objetivo, int funcionError) {

	if(bOnline)
	{
		for(int capa=1;capa<nNumCapas;capa++)
		{
			for(int neurona=0;neurona<pCapas[capa].nNumNeuronas;neurona++)
			{
				for(int k=0;k<pCapas[capa-1].nNumNeuronas;k++)
				{
					pCapas[capa].pNeuronas[neurona].ultimoDeltaW[k]=pCapas[capa].pNeuronas[neurona].deltaW[k];
					pCapas[capa].pNeuronas[neurona].deltaW[k]=0;
				}

				if(bSesgo)
				{
					pCapas[capa].pNeuronas[neurona].ultimoDeltaW[pCapas[capa-1].nNumNeuronas]=pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas];
					pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas]=0;
				}

			}
		}
	}
	alimentarEntradas(entrada);
	propagarEntradas();
	recogerSalidas(salida);
	retropropagarError(objetivo, funcionError);
	acumularCambio();
	if(bOnline)
		ajustarPesos();

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
// Entrenar la red para un determinado fichero de datos (pasar una vez por todos los patrones)
// Si es offline, después de pasar por ellos hay que ajustar pesos. Sino, ya se ha ajustado en cada patrón
void PerceptronMulticapa::entrenar(Datos* pDatosTrain, int funcionError) {

	int i;
	for(i=0; i<pDatosTrain->nNumPatrones; i++){
		double* prediccion = new double[pDatosTrain->nNumSalidas];
		simularRed(pDatosTrain->entradas[i], prediccion, pDatosTrain->salidas[i],funcionError);
		/*if(!bOnline)
		{
			ajustarPesos();
		}*/
		delete[] prediccion;
	}
	if(!bOnline)
	{
		ajustarPesos();
	}

}

// ------------------------------
// Probar la red con un conjunto de datos y devolver el error MSE cometido
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
double PerceptronMulticapa::test(Datos* pDatosTest, int funcionError) {

	int i;
	double dAvgTestError = 0;
	for(i=0; i<pDatosTest->nNumPatrones; i++){
		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas();
		dAvgTestError += calcularErrorSalida(pDatosTest->salidas[i],funcionError);
	}
	if(funcionError == 1)
	{
		return dAvgTestError /= -pDatosTest->nNumPatrones * pCapas[nNumCapas-1].nNumNeuronas;
	}
	else
	{
		return dAvgTestError /= pDatosTest->nNumPatrones;
	}

}


// ------------------------------
// Probar la red con un conjunto de datos y devolver el error CCR cometido
double PerceptronMulticapa::testClassification(Datos* pDatosTest) {

	ofstream fichero;
	fichero.open("matriz.txt");
	matriz mConfusion(pDatosTest->nNumSalidas,vectorMC(pDatosTest->nNumSalidas));

	int i;
	double ccr = 0;
	for(i=0; i<pDatosTest->nNumPatrones; i++){
		// Cargamos las entradas y propagamos el valor
		alimentarEntradas(pDatosTest->entradas[i]);
		propagarEntradas();

		//calculo del CCR

		int indiceMaximoDeseada = 0;
		int indiceMaximoObtenida = 0;
		double maximaSalida = 0.0, maximoTarget = 0.0;

		for (int n = 0; n < this->pCapas[this->nNumCapas-1].nNumNeuronas; n++)
		{
			if(pDatosTest->salidas[i][n] > maximoTarget)
			{
				maximoTarget = pDatosTest->salidas[i][n];
				indiceMaximoDeseada = n;
			}
			if(this->pCapas[this->nNumCapas-1].pNeuronas[n].x > maximaSalida)
			{
				maximaSalida = this->pCapas[this->nNumCapas-1].pNeuronas[n].x;
				indiceMaximoObtenida = n;
			}
		}
		if(indiceMaximoDeseada == indiceMaximoObtenida)
		{
			ccr++;
		}
		//fichero << i << "\t" << ccr << endl;
		if(pDatosTest->salidas[i][indiceMaximoObtenida]!=1)
		{
			fichero << "Patron " << i+1 << " digito => " << indiceMaximoObtenida << " confunde con => " << indiceMaximoDeseada << endl;

		}
		mConfusion[indiceMaximoObtenida][indiceMaximoDeseada]++;
	}
	cout << "=====================" << endl;
	for(int j = 0;j < pDatosTest->nNumSalidas; j++)
	{
		for(int k = 0; k < pDatosTest->nNumSalidas; k++)
		{
			if(k < pDatosTest->nNumSalidas-1)
				cout << mConfusion[j][k] << " - " ;
			else
				cout << mConfusion[j][k] << endl ;
		}
	}
	cout << endl;
	fichero.close();

	return 100 * (ccr/pDatosTest->nNumPatrones);

}

// ------------------------------
// Ejecutar el algoritmo de entrenamiento durante un número de iteraciones, utilizando pDatosTrain
// Una vez terminado, probar como funciona la red en pDatosTest
// Tanto el error MSE de entrenamiento como el error MSE de test debe calcularse y almacenarse en errorTrain y errorTest
// funcionError=1 => EntropiaCruzada // funcionError=0 => MSE
void PerceptronMulticapa::ejecutarAlgoritmo(Datos * pDatosTrain, Datos * pDatosTest, int maxiter, double *errorTrain, double *errorTest, double *ccrTrain, double *ccrTest, int funcionError)
{
	//ofstream fichero("datos.txt");

	int countTrain = 0;

	// Inicialización de pesos
	pesosAleatorios();

	double minTrainError = 0;
	int numSinMejorar;
	double testError = 0;

	//inicializamos el aprendizaje
	if(bOnline == 0)
	{
		for(int capa = 1; capa < this->nNumCapas; capa++)
		{
			for(int neurona = 0; neurona < this->pCapas[capa].nNumNeuronas; neurona++)
			{
				//como tenemos que coger valores de w tenemos que iterar la capa anterior
				for(int i = 0; i < this->pCapas[capa-1].nNumNeuronas; i++)
				{
					pCapas[capa].pNeuronas[neurona].ultimoDeltaW[i]=0;
					pCapas[capa].pNeuronas[neurona].deltaW[i]=0;
				}

				if(bSesgo)
				{
					pCapas[capa].pNeuronas[neurona].ultimoDeltaW[pCapas[capa-1].nNumNeuronas]=0;
					pCapas[capa].pNeuronas[neurona].deltaW[pCapas[capa-1].nNumNeuronas]=0;
				}
			}
		}
	}

	// Aprendizaje del algoritmo
	do {

		entrenar(pDatosTrain,funcionError);

		//double trainError = testClassification(pDatosTrain);
		double trainError = test(pDatosTrain,funcionError);
		if(countTrain==0 || fabs(trainError - minTrainError) > 0.00001){
			minTrainError = trainError;
			copiarPesos();
			numSinMejorar = 0;
		}
		else{
			numSinMejorar++;
		}

		if(numSinMejorar==50)
			countTrain = maxiter;

		//testError = testClassification(pDatosTest);
		testError = test(pDatosTest,funcionError);
		countTrain++;

		cout << "Iteración " << countTrain << "\t Error de entrenamiento: " << trainError << endl;
		//fichero << countTrain << "\t " << trainError << endl;

	} while ( countTrain<maxiter );

	restaurarPesos();

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
			cout << pDatosTest->salidas[i][j] << " -- " << prediccion[j] << " \\\\ " ;
		cout << endl;
		delete[] prediccion;

	}

	*errorTest=testError;
	*errorTrain=minTrainError;
	*ccrTest = testClassification(pDatosTest);
	*ccrTrain = testClassification(pDatosTrain);

	//fichero.close();

}

