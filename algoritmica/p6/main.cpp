#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct material{
	int volumen;
	float precio;
	string usado;
};

/*
	Volumen mochila = 100
	Materiales = 5
	vi = {25, 20, 25, 15, 70}
	pi = {25, 15, 30, 45, 40}

	solucion => P=3550 => (40, 70), (30, 25)
*/

void introducirMateriales(int &nmateriales, int &volumenm, vector <struct material> &materiales);
float limite(int &n, float &pActual, int &vActual, int &k, vector <struct material> &materiales, int &V);
void MochilaBacktracking(int &nmateriales, vector <struct material> &materiales, int V, int * X, float &precioFinal, int &volumenFinal);

int main()
{
	//declaramos el vector de materiales
	vector <struct material> materiales;

	int volumenm=0, nmateriales=0, volumenFinal=0;
	float precioFinal=0;

	introducirMateriales(nmateriales, volumenm, materiales);

	//Vector de resultado
	int X[nmateriales+1]; //se pone +1 para seguir el indice del pseudocodigo

	MochilaBacktracking(nmateriales, materiales, volumenm, X, precioFinal, volumenFinal);

	cout << "Volumen:" << volumenFinal << endl;
	
	cout << "Resultado: " << precioFinal << endl;

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

	materiales.push_back(materialAux); //asi lo que hago es tener un material mas y puedo seguir los indices del pseudocodigo

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

float limite(int &n, float &pActual, int &vActual, vector <struct material> &materiales, int &k, int &V)
{
	float valor = pActual;
	int volumen = vActual;

	for (int i=k+1 ; i<n ; i++)//Se irán introduciendo los materiales restantes más caros hasta llegar al volumen de la mochila
	{
		volumen += materiales[i].volumen;

		if(volumen < V)//Se puede introducir el material i completo
		{
			valor += materiales[i].precio * materiales[i].volumen;
		}
		else //Se puede introducir el material i parcialmente
		{
			return valor + (V - (volumen - materiales[i].volumen)) * materiales[i].precio;
		}
	}

	return valor;
}

void MochilaBacktracking(int &n, vector <struct material> &materiales, int V, int * X, float &precioFinal, int &volumenFinal)
{
	float precio = 0.0;
	int volumen = 0;
	precioFinal = -1.0;
	int k = 1;
	int Y[n+1];
	bool final = false;

	while(!final)
	{
		while( (k <=n) && (volumen + materiales[k].volumen <= V))//el material k entra en la mochila
		{
			volumen += materiales[k].volumen;
			precio = precio + (materiales[k].precio * materiales[k].volumen);
			Y[k] = 1;
			k ++;
		}

		if (k > n)//Se actualiza la solucion
		{
			precioFinal = precio;
			volumenFinal = volumen;
			k = n;
			for( unsigned int i = 1; i<=materiales.size(); i++)
			{
				X[i]=Y[i];
			}
		}
		else //El volumen se supera con el objeto k. Se quita el objeto k
		{
			Y[k] = 0;
		}

		//despues de que precioFinal se calcule arriba, Limite=precioFinal

		while( (limite(n, precio, volumen, materiales, k, V)) <= precioFinal )
		{
			while( k!=0 and (Y[k]!=1) )//Busca el último material de la mochila
			{
				k --;
			}

			if( k==0 )//aqui termina el algoritmo
			{
				final = true;
				break;
			}

			//Borra el k-ésimo elemento de la mochila
			Y[k] = 0;
			volumen = volumen - materiales[k].volumen;
			precio = precio - materiales[k].precio * materiales[k].volumen;
		}

		k ++; //Pasa al siguiente material
	}

}
