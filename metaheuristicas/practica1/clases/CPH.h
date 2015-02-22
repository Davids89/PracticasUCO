/*
 * CPH.h
 *
 *  Created on: 11/03/2014
 *      Author: i12luqqd
 */

#ifndef CPH_H_
#define CPH_H_

#include "Problema.h"

namespace clases
{

struct coordenadas
{
	float x;
	float y;
	float f;
};

struct datosInstanciaCPH
{
	int n, p, c;
	struct coordenadas * cooNodo;
	float ** distancias;
};

struct datosSolucionCPH
{
	bool ** matrizConectividad;
	int * vectorSolucion;
};

class CPH: public clases::Problema<struct datosInstanciaCPH,struct datosSolucionCPH>
{
public:
	CPH();
	virtual ~CPH();
	int leerInstancia(char *archivo);
	float evaluarSolucion(struct datosSolucionCPH * sol);
	struct datosSolucionCPH * generarSolucionRandom();
	void imprimirSolucion(struct datosSolucionCPH * sol);
	void imprimirInstancia();
	void liberarMemoriaSolucion(struct datosSolucionCPH *sol);
private:
	int reservarMemoria();
	int reservarMemoriaSolucion(struct datosSolucionCPH * solucion);
	void liberarMemoria();
};

}


#endif /* CPH_H_ */
