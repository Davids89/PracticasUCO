/*
 * CPH.h
 *
 *  Created on: 11/03/2014
 *      Author: i12luqqd
 */

#ifndef CPH_H_
#define CPH_H_

#include "Problema.h"
#include <iostream>
#include <string>

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
	float valorObjetivo;
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
	struct datosSolucionCPH * aplicarAlgoritmoGenetico(int tamanoPoblacion, float pCruce, float pMutacion, int nEval, bool estacionario, std::string archivo);
	int evaluacion(struct datosSolucionCPH **pob, int tamanoPoblacion);
	int actualizar();
private:
	int reservarMemoria();
	int reservarMemoriaSolucion(struct datosSolucionCPH * solucion);
	void liberarMemoria();
	void operadorMutacion(struct datosSolucionCPH *ind);
	void operadorCruce(struct datosSolucionCPH *ind1, struct datosSolucionCPH *ind2);
	struct datosSolucionCPH ** seleccionPorTorneo(struct datosSolucionCPH **pob, int tamanoTorneo, int tamanoPoblacion, int numeroSeleccion);
	struct datosSolucionCPH ** seleccionPorRuleta(struct datosSolucionCPH **pob, int tamanoPoblacion, int numeroSeleccion);
	struct datosSolucionCPH * mejorSolucionPoblacion(struct datosSolucionCPH **pob, int tamanoPoblacion);
};

}


#endif /* CPH_H_ */
