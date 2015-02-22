/*
 * TSP.h
 *
 *  Created on: 05/03/2014
 *      Author: i12luqqd
 */

#ifndef TSP_H_
#define TSP_H_

#include "Problema.h"

namespace clases{

struct datosInstanciaTSP{
	int m;
	int ** distancias;
};

struct datosSolucionTSP{
	int * vectorSolucion;
};

class TSP: public clases::Problema<struct datosInstanciaTSP,struct datosSolucionTSP> {
public:
	TSP();
	virtual ~TSP();
	int leerInstancia(char *archivo);
	float evaluarSolucion(struct datosSolucionTSP * sol);
	//float evaluarSolucion(struct datosSolucionTSP * sol);
	struct datosSolucionTSP * generarSolucionRandom();
	void imprimirSolucion(struct datosSolucionTSP * sol);
	void imprimirInstancia();
	void liberarMemoriaSolucion(struct datosSolucionTSP *sol);
private:
	int reservarMemoria();
	int reservarMemoriaSolucion(struct datosSolucionTSP * solucion);
	void liberarMemoria();
};

}

#endif /* TSP_H_ */
