/*
 * Problema.h
 *
 *  Created on: 06/02/2013
 *      Author: pedroa
 */

// Manejo de errores
// -1 error al abrir el fichero
// -2 error al leer el formato del fichero
// -3 error en la reserva de memoria
#ifndef PROBLEMA_H_
#define PROBLEMA_H_

namespace clases {

// Plantilla de clase
// Establece la estructura de las clases que representarán a los problemas (MMDP, CWP y CPH)
// Estas clases deberán heredar de Problema y definir los métodos virtuales necesarios
// Cada una de estas clases deberá definir dos estructuras, una para los datos de la instancia
// y otra para los datos de la solución. Estas estructuras son las que aparecen como argumentos
// en la plantilla.
template<typename DATOS_INSTANCIA, typename DATOS_SOLUCION>
class Problema {
public:
	//Problema();
	//~Problema()=0;
	virtual int leerInstancia(char * archivo)=0;
	//virtual float evaluarSolucion(DATOS_SOLUCION * sol, char * tipo)=0;
	virtual DATOS_SOLUCION * generarSolucionRandom()=0;
	virtual void imprimirSolucion(DATOS_SOLUCION * sol)=0;
	virtual void imprimirInstancia()=0;
	virtual void liberarMemoriaSolucion(DATOS_SOLUCION *sol)=0;
protected:
	DATOS_INSTANCIA data;
	virtual int reservarMemoria()=0;
	virtual DATOS_SOLUCION * reservarMemoriaSolucion()=0;
	virtual void liberarMemoria()=0;
};

}

#endif /* PROBLEMA_H_ */
