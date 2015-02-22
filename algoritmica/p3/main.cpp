#include <iostream>
#include "funcionesbajonivel.hpp"
#include "funcionesaltonivel.hpp"
#include <string.h>

using namespace std;

void pedirDatos(char * &numero1, char * &numero2, int ndigitos);

int main()
{

	int ndigitos=0, operacion;

	//declaracion de los vectores de caracteres

	char * numero1 = reservarCadena(ndigitos);
	char * numero2 = reservarCadena(ndigitos);
	char * totalSuma; //hay que reservar uno mas por si desborda el ultimo
	//char * totalMul;

	int limite;

	do{
		cout << "----------------------------------" << endl;
		cout << "Introduzca la operacion a realizar" << endl;
		cout << "\tSumar [1] " << endl << "\tMultiplicar [2]" << endl << "\tSalir [3]" << endl;
		cin >> operacion;

		while(operacion!=1 and operacion!=2 and operacion!=3 )
		{
			cout << "Introduzca un valor valido: " << endl;
			cin >> operacion;
		}

		switch(operacion)
		{
			case 1:
				cout << "Introduzca el numero de digitos de los numeros" << endl;
				cin >> ndigitos;

				pedirDatos(numero1, numero2, ndigitos);

				totalSuma = sumarNumero(numero1, numero2);

				quitarCerosNoSignificativos(totalSuma);

				cout << "Suma: " << totalSuma << endl;

				break;
			case 2:
				cout << "Introduzca el numero de digitos de los numeros" << endl;
				cin >> ndigitos;

				pedirDatos(numero1, numero2, ndigitos);

				cout << "Limite" << endl;

				cin >> limite;

				cout << "Multiplicacion: " << multiplicaNumeros(numero1,numero2,limite)
 << endl;

				break;
		}

	}while(operacion != 3);

	return 0;

}

void pedirDatos(char * &numero1, char * &numero2, int ndigitos)
{
	int nceros=0;	

	cout << "Introduzca numero 1: " << endl;
	cin >> numero1;

	cout << "Introduzca numero 2: " << endl;
	cin >> numero2;
	
	//Miramos los numeros que hay para ver si tenemos que agregar numeros en caso de que el que hayamos introducido sea menor del numero de digitos

	nceros = ndigitos - strlen(numero1);
	agregarCerosDelante(numero1, nceros);

	nceros = ndigitos - strlen(numero2);
	agregarCerosDelante(numero2, nceros);
}
