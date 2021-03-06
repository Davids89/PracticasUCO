#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

//Declaraciones de funciones
void rellenarVector(vector<int> &v, int n);
bool busquedaSecuencial(vector<int> &v, int n);
void calcularAjusteLineal(const vector <double> &x, const vector <double> &y, double &a, double &b, double &r2);
//el primero es elementos, el segundo es el de tiempo (tiempo_sec o tiempo_bin). Los parametros a, b y r2 se calculan en la funcion
double calcularMedia(const vector <double> &v);
double calcularCovarianza(const vector <double>&x, const double &xMedia, const vector <double> &y, const double &yMedia);
double calcularVarianza(const vector <double> &x, const double &media);
void calcularTiemposEstimados(const vector<double> &x, double a, double b, vector<double> &yEstimada);
void guardarTiempos(const vector <double> n, const vector<double> &tSecuencial, const vector<double> &tSecuencialEstimados, const vector <double> &tBinaria, const vector <double> &tBinariaEstimados, string fichero);
//n es el vector de elementos, tSecuencial el tiempo obtenido de la busqueda secuencial, tSecuencialEstimados el tiempo secuencial estimado.
//tBinaria es el tiempo obtenido de la busqueda binaria y tBinariaEstimados es el tiempo estimado de binario

int main()
{
	double tiempoMilisegundos;
	//int x = 0;
	struct timeval tInicial, tFinal;

	//variables para el vector
	int minimo, maximo, increm, nveces, elemento;
	//declaracion de vector
	vector<int> v; //v es el vector para la lineal y v_log para la logaritmica
	vector<double> tiempo_sec, tiempo_bin, elementos, logelementos, tiempo_est_sec, tiempo_est_bin;
	//guardamos en tiempo_sec y tiempo_bin los tiempos medios obtenidos de las pruebas
	//en tiempo_est_sec y tiempo_est_bin los tiempos estimados para lineal y logaritmica
	//en elementos guardamos el numero de elementos

	//semilla
	srand(time(NULL));

	//acumuladores
	double acu_secuencial=0;
	double acu_binario=0;

	//valores para el ajuste lineal
	double a_sec=0, b_sec=0, r2_sec=0;
	double a_bin=0, b_bin=0, r2_bin=0;

	//fichero
	string fichero;

	cout << "Introduce el minimo de elementos" << endl;
	cin >> minimo;
	cout << "Introduce el maximo de elementos" << endl;
	cin >> maximo;
	cout << "Introduce el incremento de los elementos" << endl;
	cin >> increm;
	cout << "Introduce las veces a repetir" << endl;
	cin >> nveces;

	for (int i=minimo; i<=maximo; i=i+increm)
	{
		//guardar elementos
		elementos.push_back(i);
		//guardar logaritmo
		logelementos.push_back(log(i));

		//limpiamos acumuladores
		acu_secuencial=0;
		acu_binario=0;

		rellenarVector(v,i);
		//cout << "Rellena vector" << endl;
		sort(v.begin(),v.end());
		//cout << "Ordena" << endl;

		// aqui va la llamada a la busqueda
		for(int j = 0; j < nveces ; j++)
		{

			//elemento a buscar
			elemento=rand()%10000;
			
			gettimeofday(&tInicial, NULL); //esto es para tomar el tiempo inicial
			
			//Busqueda secuencial
			busquedaSecuencial(v, elemento);
			
			gettimeofday(&tFinal, NULL); //esto para tomar el tiempo final

			tiempoMilisegundos = 0.001*((tFinal.tv_sec * 1000000.0 + tFinal.tv_usec) - (tInicial.tv_sec * 1000000.0 + tInicial.tv_usec));

			acu_secuencial=acu_secuencial+tiempoMilisegundos;//acumulador secuencial

			/*----------------------------------------------------------------------------------------*/
			
			gettimeofday(&tInicial, NULL); //esto es para tomar el tiempo inicial
			
			//busqueda binaria
			binary_search(v.begin(),v.end(),elemento);

			gettimeofday(&tFinal, NULL); //esto para tomar el tiempo final

			tiempoMilisegundos = 0.001*((tFinal.tv_sec * 1000000.0 + tFinal.tv_usec) - (tInicial.tv_sec * 1000000.0 + tInicial.tv_usec));			

			acu_binario=acu_binario+tiempoMilisegundos; //acumulador binario

		}

		//media secuencial
		acu_secuencial=acu_secuencial/nveces;//hacemos la media del tiempo secuencial
		tiempo_sec.push_back(acu_secuencial);//guardamos el tiempo secuencial

		//media binaria
		acu_binario=acu_binario/nveces;//hacemos la media del tiempo binario
		tiempo_bin.push_back(acu_binario);//guardamos el tiempo binario

	}

	//ajuste lineal secuencial
	calcularAjusteLineal(elementos, tiempo_sec, a_sec, b_sec, r2_sec);
	//ajuste lineal binario
	calcularAjusteLineal(logelementos, tiempo_bin, a_bin, b_bin, r2_bin);

	//tiempo estimado de secuencial
	calcularTiemposEstimados(elementos, a_sec, b_sec, tiempo_est_sec);
	//tiempo estimado de binaria
	calcularTiemposEstimados(logelementos, a_bin, b_bin, tiempo_est_bin);

	cout << "Introduzca el nombre del fichero" << endl;
	cin >> fichero;
	guardarTiempos(elementos, tiempo_sec, tiempo_est_sec, tiempo_bin, tiempo_est_bin, fichero);

	cout << "Ecuacion secuencial: t = " << a_sec << "n + " << b_sec << endl;
	cout << "Coeficiente de determinación secuencial: " << r2_sec << endl;

	cout << "Ecuacion binaria: t = " << a_bin << "log(n) + " << b_bin << endl;
	cout << "Coeficiente de determinación binario: " << r2_bin << endl;

	return 0;
}

void rellenarVector(vector<int> &v, int n)
{
	v.clear();
	for (int i=0; i<n; i++)
	{
		v.push_back(rand()%10000);
	}
}

bool busquedaSecuencial(vector<int> &v, int n)
{
	for(int i = 0; i < v.size(); i++)
	{
		if(v[i]==n)
		{
			return true;
		}
	}

	return false;
}

void calcularAjusteLineal(const vector <double> &x, const vector <double> &y, double &a, double &b, double &r2)
{
	//La ecuacion es de la forma y=ax + b
	//a = covarianza (x,y)
	//b = media(y) - a*media(x)

	double mediaX, mediaY, coV, varX, varY;

	mediaX=calcularMedia(x);//media de x
	mediaY=calcularMedia(y);//media de y
	coV=calcularCovarianza(x, mediaX, y, mediaY);//covarianza xy
	varX=calcularVarianza(x,mediaX);//varianza de x
	varY=calcularVarianza(y,mediaY);//varianza de y

	
	a=coV/varX;//calculando a
	b=mediaY - (a*mediaX);//calculo de b
	r2=(coV*coV)/(varX*varY);//calculo de r2
}

double calcularMedia(const vector <double> &v)
{
	double acu=0;
	for(int i=0; i < v.size(); i++)
	{
		acu = acu + v[i];
	}
	acu = acu / v.size();

	return acu;
}

double calcularCovarianza(const vector <double>&x, const double &xMedia, const vector <double> &y, const double &yMedia)
{
	double acu=0;
	
	for(int i=0; i < x.size(); i++)
	{
		acu = acu + ((x[i]-xMedia)*(y[i]-yMedia));
	}
	acu = acu / x.size();
	return acu;
}

double calcularVarianza(const vector <double> &x, const double &media)
{
	double var = 0;
	for(int i=0; i < x.size(); i++)
	{
		var=pow((x[i]-media),2)+var;
	}
	var = var / x.size();
	return var;
}

void calcularTiemposEstimados(const vector<double> &x, double a, double b, vector<double> &yEstimada)
{
	//y=ax+b

	for(int i=0; i<x.size(); i++)
	{
		yEstimada.push_back(x[i]*a+b);
	}
}

void guardarTiempos(const vector <double> n, const vector<double> &tSecuencial, const vector<double> &tSecuencialEstimados, const vector <double> &tBinaria, const vector <double> &tBinariaEstimados, string fichero)
{
	ofstream flujoSalida;
	flujoSalida.open(fichero.c_str(),ios::out);

	if(!flujoSalida)
	{
		cout << "Error al abrir fichero" << endl;
	}

	for(int i=0;i < n.size(); i++)
	{
		flujoSalida << n[i] << "\t" << tSecuencial [i] << "\t" << tSecuencialEstimados[i] << "\t" << tBinaria[i] << "\t" << tBinariaEstimados[i] << endl;
	}
	flujoSalida.close();

}
