#include <iostream>
#include <cmath>
#include "matriz.hpp"
#include <vector>
#include <time.h>
#include <sys/time.h>
#include <fstream>

using namespace std;
using namespace algoritmica;

template <class T> void imprimirMatriz(const Matriz <T> &m);
template <class T> T determinanteIterativo(Matriz <T> &m);
template <class T> T determinanteRecursivo(Matriz <T> &m);

//funciones para los tiempos
void calcularAjusteLineal(const vector <double> &x, const vector <double> &y, double &a, double &b, double &r2);
//el primero es elementos, el segundo es el de tiempo (tiempo_sec o tiempo_bin). Los parametros a, b y r2 se calculan en la funcion
double calcularMedia(const vector <double> &v);
double calcularCovarianza(const vector <double>&x, const double &xMedia, const vector <double> &y, const double &yMedia);
double calcularVarianza(const vector <double> &x, const double &media);
void calcularTiemposEstimados(const vector<double> &x, double a, double b, vector<double> &yEstimada);
void guardarTiempos(const vector <double> n, const vector<double> &tSecuencial, const vector<double> &tSecuencialEstimados, const vector <double> &tBinaria, const vector <double> &tBinariaEstimados, string fichero);
double factorial(double n);

int main()
{

	//variable para los tiempos
	double tiempoMilisegundos;

	//estructura de tiempos
	struct timeval tInicial, tFinal;

	//valores para el ajuste lineal
	double a_it=0, b_it=0, r2_it=0;
	double a_rec=0, b_rec=0, r2_rec=0;

	//fichero
	string fichero;

	//variables para la matriz
	int orden=0, inf=0, sup=0;

	//acumuladores
	double acu_it=0;
	double acu_rec=0;

	//declaracion de vector
	vector<double> elementos;
	vector<double> tiempo_rec, tiempo_it, ncubo, nfac, tiempo_est_rec, tiempo_est_it;

	cout << "Introduzca el orden maximo de la matriz" << endl;
	cin >> orden;
	cout << "Intervalo inferior de valores" << endl;
	cin >> inf;
	cout << "Intervalo superior de valores" << endl;
	cin >> sup;
	
	for(int i=2; i<=orden; i++)
	{
		ncubo.push_back(pow(i,3));
		nfac.push_back(factorial(i));
		elementos.push_back(i);

		//limpiamos acumuladores
		acu_it=0;
		acu_rec=0;
	
		//declaracion de la matriz
		Matriz <double> m(i,i);

		double valorIterativo=0.0;
		double valorRecursivo=0.0;

		//cerr << "Asignado el orden" << endl;

		m.rellenarMatriz(inf, sup);

		//calculamos los tiempos del iterativo

		gettimeofday(&tInicial, NULL); //esto es para tomar el tiempo inicial

		valorIterativo=determinanteIterativo(m);

		gettimeofday(&tFinal, NULL); //esto para tomar el tiempo final

		tiempoMilisegundos = 0.001*((tFinal.tv_sec * 1000000.0 + tFinal.tv_usec) - (tInicial.tv_sec * 1000000.0 + tInicial.tv_usec));

		acu_it=acu_it+tiempoMilisegundos;//acumulador secuencial

		tiempo_it.push_back(acu_it);

		
		//calculamos los tiempos recursivos

		gettimeofday(&tInicial, NULL); //esto es para tomar el tiempo inicial

		valorRecursivo=determinanteRecursivo(m);

		gettimeofday(&tFinal, NULL); //esto para tomar el tiempo final

		tiempoMilisegundos = 0.001*((tFinal.tv_sec * 1000000.0 + tFinal.tv_usec) - (tInicial.tv_sec * 1000000.0 + tInicial.tv_usec));

		acu_rec=acu_rec+tiempoMilisegundos;//acumulador secuencial

		tiempo_rec.push_back(acu_rec);
	}

	//ajuste lineal iterativo
	calcularAjusteLineal(ncubo, tiempo_it, a_it, b_it, r2_it);
	//ajuste lineal recursivo
	calcularAjusteLineal(nfac, tiempo_rec, a_rec, b_rec, r2_rec);

	//tiempo estimado de secuencial
	calcularTiemposEstimados(ncubo, a_it, b_it, tiempo_est_it);
	//tiempo estimado de binaria
	calcularTiemposEstimados(nfac, a_rec, b_rec, tiempo_est_rec);

	cout << "Introduzca el nombre del fichero" << endl;
	cin >> fichero;
	guardarTiempos(elementos, tiempo_it, tiempo_est_it, tiempo_rec, tiempo_est_rec, fichero);

	cout << "Iterativo: t = " << a_it << "n + " << b_it << endl;
	cout << "Coeficiente de iterativo: " << r2_it << endl;

	cout << "Recursivo: t = " << a_rec << "n + " << b_rec << endl;
	cout << "Coeficiente de recursivo: " << r2_rec << endl;

	return 0;

}

template <class T> T determinanteIterativo(Matriz <T> &m)
{
	int i,j,k,a;

	T pivote=0.0, valor=0.0;
	bool noCero=false;
	T determinante=1.0;
	Matriz <double> maux(m);

	for(i=1;i<=maux.filas();i++)
	{
		noCero=false;
		a=i;//para recorrer la fila
		while(noCero==false)
		{
			if(maux.elemento(a,i)!=0)
				noCero=true;
			else
				a++;
		}
		pivote=maux.elemento(a,i); //hemos encontrado el pivote
		for(j=i+1;j<=maux.columnas();j++)
		{
			valor=maux.elemento(j,i);
			for(k=i;k<=maux.columnas();k++)
			{
				maux.elemento(j,k,maux.elemento(j,k)-((valor/pivote)*maux.elemento(i,k)));
			}
		}
		
	}

	for(i=1; i<=maux.filas(); i++)
	{
		determinante = determinante * maux.elemento(i,i);
	}

	return determinante;
 
}// fin determinanteIterativo

template <class T> T determinanteRecursivo(Matriz <T> &m)
{

	T determinante=0.0;
	int aux=1;
	Matriz <double> auxiliar(m.filas()-1,m.columnas()-1);

	if(m.filas()>2)
	{
		determinante=0.0;
		for(int a=1;a<=m.columnas();a++)
		{
			for(int i=2;i<=m.filas();i++)
			{
				aux=1;
				for(int j=1;j<=m.columnas();j++)
				{
					if(j!=a)
					{
						auxiliar.elemento(i-1,aux,m.elemento(i,j));
						aux++;
					}
				}
			}
			determinante += pow(-1,1+a) * m.elemento(1,a) * determinanteRecursivo(auxiliar);
		}
	}
	else
		if(m.filas()==2)
			return (m.elemento(1,1)*m.elemento(2,2)) - (m.elemento(1,2)*m.elemento(2,1));
		else
			return m.elemento(1,1);

	return determinante;
}//fin determinanteRecursivo

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
	for(double i=0.0; i < v.size(); i++)
	{
		acu = acu + v[i];
	}
	acu = acu / v.size();

	return acu;
}

double calcularCovarianza(const vector <double>&x, const double &xMedia, const vector <double> &y, const double &yMedia)
{
	double acu=0;
	
	for(double i=0.0; i < x.size(); i++)
	{
		acu = acu + ((x[i]-xMedia)*(y[i]-yMedia));
	}
	acu = acu / x.size();
	return acu;
}

double calcularVarianza(const vector <double> &x, const double &media)
{
	double var = 0;
	for(double i=0.0; i < x.size(); i++)
	{
		var=pow((x[i]-media),2)+var;
	}
	var = var / x.size();
	return var;
}

void calcularTiemposEstimados(const vector<double> &x, double a, double b, vector<double> &yEstimada)
{
	//y=ax+b

	for(double i=0.0; i<x.size(); i++)
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

	for(double i=0.0;i < n.size(); i++)
	{
		flujoSalida << n[i] << "\t" << tSecuencial [i] << "\t" << tSecuencialEstimados[i] << "\t" << tBinaria[i] << "\t" << tBinariaEstimados[i] << endl;
	}
	flujoSalida.close();

}

double factorial(double n)
{
	if (n==1)
		return 1;
	else
		return n*factorial(n-1);
}
