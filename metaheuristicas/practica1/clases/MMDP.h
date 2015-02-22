/*
 * MMDP.h
 *
 *  Created on: 21/02/2014
 *      Author: pedroa
 */

#ifndef MMDP_H_
#define MMDP_H_

#include "Problema.h"

namespace clases {

struct datosInstanciaMMDP{
	int n, m;
	float ** distancia;
};

struct datosSolucionMMDP{
	bool * vectorSolucion;
};

class MMDP: public clases::Problema<struct datosInstanciaMMDP,struct datosSolucionMMDP> {

public:
	MMDP();
	virtual ~MMDP();
	int leerInstancia(char *archivo);
	float evaluarSolucion(struct datosSolucionMMDP* sol);
	struct datosSolucionMMDP * generarSolucionRandom();
	void imprimirSolucion(struct datosSolucionMMDP * sol);
	void imprimirInstancia();
	void liberarMemoriaSolucion(struct datosSolucionMMDP *sol);
private:
	int reservarMemoria();
	int reservarMemoriaSolucion(struct datosSolucionMMDP * solucion);
	void liberarMemoria();
};

}

#endif /* MMDP_H_ */
