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

class MMDP: public clases::Problema<struct datosInstanciaMMDP,struct datosSolucionMMDP>{
public:
	MMDP();
	virtual ~MMDP();
	int leerInstancia(char *archivo);
	float evaluarSolucion(struct datosSolucionMMDP* sol);
	struct datosSolucionMMDP * generarSolucionRandom();
	void imprimirSolucion(struct datosSolucionMMDP * sol);
	void imprimirInstancia();
	void liberarMemoriaSolucion(struct datosSolucionMMDP *sol);
	struct datosSolucionMMDP * intercambio(struct datosSolucionMMDP * solucion, int i, int j);
	struct datosSolucionMMDP * generarVecino(struct datosSolucionMMDP * solucion, int indice);
	struct datosSolucionMMDP * aplicarBusquedaLocal(struct datosSolucionMMDP *sol, bool firstImprovement, int iterRelaTamano);
	struct datosSolucionMMDP * aplicarEnfriamientoSimulado(struct datosSolucionMMDP *sol, float tFinal, float gamma, int iterRelaTamano);
private:
	int reservarMemoria();
	struct datosSolucionMMDP * reservarMemoriaSolucion();
	void liberarMemoria();
	char * tipo;
};

}

#endif /* MMDP_H_ */
