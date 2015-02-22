#ifndef __ALGORITMOSGRAFOS_H__
#define __ALGORITMOSGRAFOS_H__
#include <iostream>

#include "grafo.hpp"
using namespace std;

static const int INFINITO = 32000;

template <class G_Nodo, class G_Lado>
class AlgoritmosGrafos
{
  public:
    //Al definirlas como estáticas pueden ser invocadas sin crear un objeto de la clase AlgoritmosGrafos.
		//Dejar solo la funcion correspondiente al problema que hagais.
		static void kruskal(const GrafoDenso<G_Nodo, G_Lado> g,  const GrafoDenso<G_Nodo, G_Lado> &grafoSolucion)
		{
		};


		static void viajanteComercio(const GrafoDenso<G_Nodo, G_Lado> g,  const GrafoDenso<G_Nodo, G_Lado> &grafoSolucion)
		{
		};

};

#endif
