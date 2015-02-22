/*
 * TSP.h
 *
 *  Created on: 05/03/2014
 *      Author: i12luqqd
 */

#ifndef TSP_H_
#define TSP_H_

#include "Problema.h"
#include <vector>

namespace clases{

struct datosInstanciaTSP{
	int m;
	float ** distancias;
};

struct datosSolucionTSP{
	int * vectorSolucion;
};

class TSP: public clases::Problema<struct datosInstanciaTSP,struct datosSolucionTSP>{
public:
	TSP();
	virtual ~TSP();
	int leerInstancia(char *archivo);
	float evaluarSolucion(struct datosSolucionTSP * sol);
	struct datosSolucionTSP * generarSolucionRandom();
	void imprimirSolucion(struct datosSolucionTSP * sol);
	void imprimirInstancia();
	void liberarMemoriaSolucion(struct datosSolucionTSP *sol);
	void intercambio(struct datosSolucionTSP * solucion, int i);
	struct datosSolucionTSP * generarVecino(struct datosSolucionTSP * sol, int indice, int tipo);
	struct datosSolucionTSP * aplicarEnfriamientoSimulado(struct datosSolucionTSP *sol, float tFinal, float gamma, int iterRelaTamano);
	struct datosSolucionTSP * aplicarBusquedaLocal(struct datosSolucionTSP *sol,  bool firstImprovement, int iterRelaTamano);
	int * CVF(datosSolucionTSP * sol);
	struct datosSolucionTSP * opt(struct datosSolucionTSP * solucion, int indice1, int indice2);
private:
	int reservarMemoria();
	datosSolucionTSP * reservarMemoriaSolucion();
	void liberarMemoria();
	char * tipo;
};

}

#endif /* TSP_H_ */
