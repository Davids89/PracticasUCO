/*********************************************************************
 * File  : PerceptronMulticapa.cpp
 * Date  : 2013
 *********************************************************************/


#ifndef _PERCEPTRONMULTICAPA_H_
#define _PERCEPTRONMULTICAPA_H_

namespace redneuronal{

// Estructuras sugeridas
// ---------------------
struct Neurona {
	//la dimesion de los vectores es el numero de neuronas de capa anterior
	double  x;            /* Salida producida por la neurona (out_j^h)*/
	double  dX;           /* Derivada de la salida producida por la neurona (delta_j)*/
	double* w;            /* Vector de pesos de entrada (w_{ji}^h)*/
	double* deltaW;       /* Cambio a aplicar a cada peso de entrada (\Delta_{ji}^h (t))*/
	double* ultimoDeltaW; /* Último cambio aplicada a cada peso (\Delta_{ji}^h (t-1)) para el factor de momento*/
	double* wCopia;       /* Copia de los pesos de entrada */
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

class PerceptronMulticapa {
private:
	int    nNumCapas;     /* Número de capas total en la red */
	Capa* pCapas;         /* Vector con cada una de las capas */

	// Liberar memoria para las estructuras de datos
	void liberarMemoria();

	// Rellenar todos los pesos (w) aleatoriamente entre -1 y 1
	void pesosAleatorios();

	// Alimentar las neuronas de entrada de la red con un patrón pasado como argumento
	void alimentarEntradas(double* entrada);

	// Recoger los valores predichos por la red (out de la capa de salida) y almacenarlos en el vector pasado como argumento
	void recogerSalidas(double* salida);

	// Hacer una copia de todos los pesos (copiar w en copiaW)
	void copiarPesos();

	// Restaurar una copia de todos los pesos (copiar copiaW en w)
	void restaurarPesos();

	// Calcular y propagar las salidas de las neuronas, desde la primera capa hasta la última
	void propagarEntradas();

	// Calcular el error de salida (MSE) del out de la capa de salida con respecto a un vector objetivo y devolverlo
	double calcularErrorSalida(double* objetivo);

	// Retropropagar el error de salida con respecto, desde la última capa hasta la primera
	void retropropagarError(double* objetivo);

	// Acumular los cambios producidos por un patrón en deltaW
	void acumularCambio();

	// Actualizar los pesos de la red, desde la primera capa hasta la última
	void ajustarPesos();

	// Imprimir la red, es decir, todas las matrices de pesos
	void imprimirRed();

	// Simular la red: propragar las entradas hacia delante, computar el error, retropropagar el error y ajustar los pesos
	// entrada es el vector de entradas del patrón, objetivo es el vector de salidas deseadas del patrón, y salida deberá
        // ser utilizado para almacenar la predicción realizada por la red
	void simularRedOnline(double* entrada, double* salida, double* objetivo);


public:
	// Valores de parámetros (son públicos, para que puedan ser actualizados desde fuera)
	double dEta;        // Tasa de aprendizaje
	double dMu;         // Factor de momento
	bool   bSesgo;      // ¿Van a tener sesgo las neuronas?

	// Constructor
	PerceptronMulticapa();

	// Destructor
	~PerceptronMulticapa();

	// Reservar memoria para las estructuras de datos
        // nl tiene el numero de capas y npl es un vector que contiene el número de neuronas por cada una de las capas
        // Rellenar vector Capa* pCapas
	int inicializar(int nl, int npl[]);

	// Leer una matriz de datos a partir de un nombre de fichero y devolverla
	Datos* leerDatos(const char *archivo);

	// Probar la red con un conjunto de datos y devolver el error MSE cometido
	double test(Datos* pDatosTest);

	// Entrenar la red on-line para un determinado fichero de datos
	void entrenarOnline(Datos* pDatosTrain);

	// Ejecutar el algoritmo de entrenamiento durante un número de iteraciones, utilizando pDatosTrain
        // Una vez terminado, probar como funciona la red en pDatosTest
        // Tanto el error MSE de entrenamiento como el error MSE de test debe calcularse y almacenarse en errorTrain y errorTest
	void ejecutarAlgoritmoOnline(Datos * pDatosTrain, Datos * pDatosTest, int maxiter, double *errorTrain, double *errorTest);

};

};

#endif
