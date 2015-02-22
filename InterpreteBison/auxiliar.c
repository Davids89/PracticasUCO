#include "auxiliar.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void hacerMinuscula(char * yytext)
{
	char *i;
	for(i = yytext; *i; i++)
	{
		*i = tolower((unsigned char)*i);
	}
}

char * quitarComillas(char * cadena)
{
	char * devuelvo;
	devuelvo = (char *)malloc(strlen(cadena));
	int i;

	int inicio, fin;

	if(cadena[0] == "\'")
	{
		for(i=1; i<strlen(cadena)-1; i++)
		{
			devuelvo[i-1] == cadena[i];
		}
	}
	else
	{
		for(i = 0; i<strlen(cadena); i++)
		{
			devuelvo[i] = cadena[i];
		}
	}

	return devuelvo;
}