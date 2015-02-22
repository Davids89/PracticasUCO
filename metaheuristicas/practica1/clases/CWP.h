/*
 * CWP.h
 *
 *  Created on: 09/03/2014
 *      Author: david
 */

#ifndef CWP_H_
#define CWP_H_

#include "Problema.h"

namespace clases
{

struct datosInstanciaCWP
{
	int n, m; //la n son los nodos y la m las aristas
	bool ** distancias;
};

struct datosSolucionCWP
{
	int * vectorSolucion;
};

class CWP: public clases::Problema<struct datosInstanciaCWP,struct datosSolucionCWP>
{
public:
	CWP();
	virtual ~CWP();
	int leerInstancia(char *archivo);
	float evaluarSolucion(struct datosSolucionCWP * sol);
	struct datosSolucionCWP * generarSolucionRandom();
	void imprimirSolucion(struct datosSolucionCWP * sol);
	void imprimirInstancia();
	void liberarMemoriaSolucion(struct datosSolucionCWP *sol);
private:
	int reservarMemoria();
	int reservarMemoriaSolucion(struct datosSolucionCWP * solucion);
	void liberarMemoria();
};

}

#endif /* CWP_H_ */
