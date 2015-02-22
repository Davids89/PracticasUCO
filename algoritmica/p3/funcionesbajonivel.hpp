#ifndef FUNCIONES_BAJO_NIVEL
#define FUNCIONES_BAJO_NIVEL

//Esta función reserva una cadena de n caracteres y devuelve un puntero a la misma.
char * reservarCadena(int n);

//Esta función divide la cadena c en dos subcadenas c1 y c2, siendo ambas del mismo tamaño cuando el número de caracteres de c es par 
//y c1 tendrá un caracter más cuando c tenga un número impar de caracteres.
void partirCadena(char *c, char* &c1, char* &c2);

//Esta función añade tantos ceros como indica nCeros al final del número almacenado en la cadena c
void agregarCerosFinal(char* &c, int nCeros);

//Esta función añade tantos ceros como indica nCeros al principio del número almacenado en la cadena c
void agregarCerosDelante(char* &c, int nCeros);

//Esta función elimina todos los ceros que hay al principio del número almacenado en la cadena c
void quitarCerosNoSignificativos(char* &c);

//Esta función devuelve una cadena que contendrá el número resultante de multiplicar el número almacenado en la cadena c por 10 elevado a potencia.
char * multiplicarPotencia10(char *c, int potencia);

#endif

