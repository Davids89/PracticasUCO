/*
 * AlgoritmoGenetico.h
 *
 *  Created on: 06/02/2014
 *      Author: pedroa
 */

// Manejo de errores
// -1 error al abrir el fichero
// -2 error al leer el formato del fichero
// -3 error en la reserva de memoria
#ifndef ALGORITMOGENETICO_H_
#define ALGORITMOGENETICO_H_

namespace clases {

template<typename DATOS_SOLUCION>
class AlgoritmoGenetico {
public:
	virtual DATOS_SOLUCION * aplicarAlgoritmoGenetico(int tamanoPoblacion, float pCruce, float pMutacion, int nEval, bool estacionario)=0;

private:
	virtual void operadorMutacion(DATOS_SOLUCION *ind)=0;
	virtual void operadorCruce(DATOS_SOLUCION *ind1,DATOS_SOLUCION *ind2)=0;
	virtual DATOS_SOLUCION ** seleccionPorTorneo(DATOS_SOLUCION **pob, int tamanoTorneo, int tamanoPoblacion, int numeroSeleccion)=0;
	virtual DATOS_SOLUCION ** seleccionPorRuleta(DATOS_SOLUCION **pob, int tamanoPoblacion, int numeroSeleccion)=0;
	virtual DATOS_SOLUCION * mejorSolucionPoblacion(DATOS_SOLUCION **pob, int tamanoPoblacion)=0;

protected:
	~AlgoritmoGenetico(void){};
};

}

#endif /* ALGORITMOGENETICO_H_ */
