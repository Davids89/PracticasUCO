#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include <sys/time.h>

using namespace std;

struct material{
	int precio;
	int volumen;
	string usado;
};

void introducirDatos(int &volumenm, int &nmateriales, vector <struct material> &materiales);
void mochila(int &nmateriales, int volumenm, vector <struct material> &materiales);
void mostrarMateriales(vector <struct material> &materiales);

bool comparacion(const material& m1, const material& m2)
{
	if(m1.precio > m2.precio)
		return true;
	else
		return false;
}

int main()
{

	int volumenm=0, nmateriales=0;
	vector <struct material> materiales;

	srand(time(NULL));

	introducirDatos(volumenm, nmateriales, materiales);

	cout << "Ordenamos mochila" << endl;
	mochila(nmateriales, volumenm, materiales);
	mostrarMateriales(materiales);

	return 0;
}

void introducirDatos(int &volumenm, int &nmateriales, vector <struct material> &materiales)
{

	cout << "Introduce el volumen de la mochila" << endl;
	cin >> volumenm;

	cout << "Introduce el numero de materiales" << endl;
	cin >> nmateriales;

	material materialaux;

	int aux;

	for(int i=0; i<nmateriales; i++)
	{
		//aux = rand()%volumenm;
		cout << "Introduzca volumen del material " << i+1 << endl;
		cout << "--------------------------------" << endl;
		cin >> aux;
		materialaux.volumen = aux;

		//aux = rand()%10;
		cout << "Introduzca precio del material " << i+1 << endl;
		cout << "--------------------------------" << endl;
		cin >> aux;
		materialaux.precio = aux;

		materiales.push_back(materialaux);
	}

	sort(materiales.begin(), materiales.end(), comparacion);
}

void mostrarMateriales(vector <struct material> &materiales)
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

void mochila(int &nmateriales, int volumenm, vector <struct material> &materiales)
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
