#include "funcionesaltonivel.hpp"
#include "funcionesbajonivel.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

using namespace std;

char * sumarNumero(char * numero1, char * numero2)
{
	int digitos; //esto es para la dimension
	int x, y, aux, aux2=0; //para sumar

	int n1 = strlen(numero1);
	int n2 = strlen(numero2);
	if(n1>n2)
	{
		agregarCerosDelante(numero2, n1-n2);// esta cadena es para el caso en que haya sobreflujo en la ultima suma
		digitos=strlen(numero1);
	}
	else
	{
		agregarCerosDelante(numero1, n2-n1);
		digitos=strlen(numero2);
	}

	char * total = reservarCadena(digitos);
	char * total2 = reservarCadena(digitos+1);
        
        total[digitos] = '\0';
        total2[digitos+1] = '\0';
	for(int i=digitos-1; i>=0; i--)
	{
		x=numero1[i]-'0'; //pasamos el numero digito a digito a entero para poder operar con el
		y=numero2[i]-'0';

		aux=x+y+aux2; //aux2 en la primera iteración vale 0 porque no nos llevamos ninguna. Si en algun momento desborda la suma se pondra a 1 y entonces así controlamos que el siguiente numero sume 1

		if(aux>=10)
		{
			aux2=1;
			aux=aux%10;
		}
		else
		{
			aux2=0;
		}

		total[i]='0'+aux; //convertimos el digito a caracter y lo introducimos en la cadena

		if(i==0 and aux2==1)
		{
			strcpy(total2,"1"); //le introducimos el 1 del sobreflujo
			total = strcat(total2,total); //concatenamos para tener el numero mas su sobreflujo
			quitarCerosNoSignificativos(total2);
			return total2;
		}

		
	}
	quitarCerosNoSignificativos(total);
	return total;
}

char * multiplicaNumeros(char * numero1, char * numero2, int limite)
{
	//declaración de los numeros con los que operaremos
	int n = strlen(numero1);

	

	//variables para multiplicar
	int s;
	char *w,*x,*y,*z; //almacena digitos que quedan

	//enteros finales
	char * final1, * final2, * final3; //sirven para la hora de retornar ya que dividimos la operacion en 3 y la sumamos
	

	

	//operación de multiplicación

	if(n <= limite)
	{
		char * total = reservarCadena(limite);
		int numeroInt1,numeroInt2, aux; //aux nos servira para pasar el numero a cadena
		numeroInt1 = atoi(numero1);
		numeroInt2 = atoi(numero2);

		aux = numeroInt1 * numeroInt2; //multiplicamos los numeros y los almacenamos en aux para ahora convertirlo en cadena con sprintf
		sprintf(total, "%d", aux);
		quitarCerosNoSignificativos(total);
		return total;
	}
	else
	{
		s=n/2;
		partirCadena(numero1, w,x);
		partirCadena(numero2,y,z);

		//ahora dividimos la operación del algoritmo en tres partes, hay que tener cuidado de convertir a entero los numeros que retorna la llamada de la multiplicación

		final1=multiplicarPotencia10(multiplicaNumeros(w,y,limite),2*s);
		final2=multiplicarPotencia10(sumarNumero(multiplicaNumeros(w,z,limite) , multiplicaNumeros(x,y,limite)),s);
		final3=multiplicaNumeros(x,z,limite);
		return (sumarNumero(sumarNumero(final1,final2),final3));

		//return total;
	}
}
