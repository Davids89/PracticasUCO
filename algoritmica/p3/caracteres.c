#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char a = '3';
	char b = '5';
	int x, y, aux;
	
	char suma;
	
	x = a - '0';
	y = b - '0';
	
	aux = x + y;
	
	printf("%d + %d = %d \n", x,y,aux);
	
	if (aux < 10)
	{
		suma = '0' + aux;
		printf("%c + %c = %c \n", a, b, suma);
	}
	else
	{
		suma = '0' + aux % 10;
		printf("%c + %c = 1%c \n", a, b, suma);
	}
		
	return 0;
}
