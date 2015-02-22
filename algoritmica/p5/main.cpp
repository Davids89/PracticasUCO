#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "matriz.hpp"

/*
	Volumen mochila = 100
	Materiales = 5
	vi = {25, 20, 25, 15, 70}
	pi = {25, 15, 30, 45, 40}

	solucion => P=3550 => (40, 70), (30, 25)
*/

using namespace std;
const int maximo = 100;

struct material{
	int volumen;
	float precio;
	string usado;
};

void introducirMateriales(int &nmateriales, int &volumenm, vector <struct material> &materiales);
void mochilaDinamica(int &nmateriales, int volumenm, vector <struct material> &materiales, float C[][maximo]);
void mostrarMaterialesDinamico(vector <struct material> &materiales, float C[][maximo], int &nmateriales, int &volumenm);

void mostrarMaterialesVoraz(vector <struct material> &materiales);
void mochilaVoraz(int &nmateriales, int volumenm, vector <struct material> &materiales);

bool comparacion(const material& m1, const material& m2)
{
	if(m1.precio > m2.precio)
		return true;
	else
		return false;
}

int main()
{

	//declaramos el vector de materiales
	vector <struct material> materiales;

	int volumenm=0, nmateriales=0;

	introducirMateriales(nmateriales, volumenm, materiales);

	//Metodo dinamico

	float C[nmateriales+1][maximo];

	mochilaDinamica(nmateriales, volumenm, materiales, C);

	cout << "-----------------------" << endl;
	cout << "Metodo Dinamico" << endl;
	cout << "-----------------------" << endl;

	mostrarMaterialesDinamico(materiales, C, nmateriales, volumenm);

	cout << endl;

	//Metodo voraz

	mochilaVoraz(nmateriales, volumenm, materiales);

	//Mostrar materiales

	cout << "-----------------------" << endl;
	cout << "Metodo Voraz" << endl;
	cout << "-----------------------" << endl;

	mostrarMaterialesVoraz(materiales);


	return 0;
}

void introducirMateriales(int &nmateriales, int &volumenm, vector <struct material> &materiales)
{

	float aux;

	cout << "Introduzca el volumen de la mochila" << endl;
	cin >> volumenm;

	cout << "Introduce el número de materiales" << endl;
	cin >> nmateriales;

	material materialAux;

	for(int i=0; i<nmateriales; i++)
	{
		cout << "Introduce el volumen del material " << i+1 << endl;
		cin >> aux;
		materialAux.volumen=aux;

		cout << "Introduce el precio del material " << i+1 << endl;
		cin >> aux;
		materialAux.precio=aux;

		materiales.push_back(materialAux);
	}
}

void mochilaDinamica(int &nmateriales, int volumenm, vector <struct material> &materiales, float C[][maximo])
{
	for(int j=1; j<=volumenm; j++)
	{
		C[0][j]=0;
	}

	for(int i=1; i<=nmateriales; i++)
	{
		C[i][0]=0;
	}

	for(int i=1; i<=nmateriales; i++)
	{
		for(int j=1; j<=volumenm; j++)
		{
			if(j < materiales[i-1].volumen)
			{
				C[i][j]=C[i-1][j];
			}else
			{
				C[i][j] = max(C[i-1][j], materiales[i-1].precio * materiales[i-1].volumen + C[i-1][j - materiales[i-1].volumen]);
			}
		}
	}
}

void mochilaVoraz(int &nmateriales, int volumenm, vector <struct material> &materiales)
{
	int resto = volumenm;
	int precioMaximo;
	int materialMaximo;
	bool materialDisponible;

	unsigned int n=materiales.size();

	//Marcamos los elementos como no elegidos

	for(unsigned int i = 0; i<n; i++)
	{
		materiales[i].usado = "Nada";
	}

	do{
		precioMaximo=0;
		materialMaximo=0;
		materialDisponible=false;

		//cerr << "1" << endl;

		//Se selecciona el  material de maximo coste
		for (unsigned int i = 0; i<n; i++)
		{
			if(materiales[i].usado == "Nada")
			{
				materialDisponible = true;
				if(materiales[i].precio > precioMaximo)
				{
					precioMaximo = materiales[i].precio;
					materialMaximo = i;
				}
			}
		}

		//cerr << "2" << endl;

		//comprobamos si el material de maximo coste cabe en la mochila

		if(materialDisponible == true)
		{
			if(resto >= materiales[materialMaximo].volumen)
			{
				materiales[materialMaximo].usado = "Usado";
				resto = resto - materiales[materialMaximo].volumen;
			}
			else
			{
				materiales[materialMaximo].usado = "Parcial";
				materiales[materialMaximo].volumen = resto; //se añade esto para cuando se muestre salga cuanta parte se introduce en la mochila
				resto = 0;
				materialDisponible = false;
			}
		}

		//cerr << "3" << endl;

	}while(materialDisponible == true);
}

void mostrarMaterialesDinamico(vector <struct material> &materiales, float C[][maximo], int &nmateriales, int &volumenm)
{

	int i = nmateriales;
	int j = volumenm;

	do
	{
		if(C[i][j] != C[i-1][j]) //directamente lo cogemos
		{
			cout << "\t Material: " << i << "\n\t Precio: " << materiales[i-1].precio << "\n\t Volumen: " << materiales[i-1].volumen << endl;
			cout << endl;
			j -= materiales[i-1].volumen;
			i--;
		}
		else
			i--;
		
	}while(j>1 and i>1);

	cout << endl << "Solucion: " << C[nmateriales][volumenm] << endl << endl;
}

void mostrarMaterialesVoraz(vector <struct material> &materiales)
{
	int maximo=0;
	for(unsigned int i=0; i<materiales.size(); i++)
	{
		if(materiales[i].usado != "Nada")
		{
			cout << "-------------------" << endl;
			cout << "Material numero: " << i << endl;
			cout << "\t Volumen material: " << materiales[i].volumen << endl;
			cout << "\t Precio material: " << materiales[i].precio << endl;
			cout << "\t Usado: " << materiales[i].usado << endl;
			maximo = maximo + materiales[i].volumen * materiales[i].precio;
		}
	}
	cout << "-------------------" << endl;
	cout << "Coste máximo de la mochila: " << maximo << endl;
	cout << "-------------------" << endl;
}