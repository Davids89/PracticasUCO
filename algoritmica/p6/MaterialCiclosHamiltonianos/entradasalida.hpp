#ifndef __ENTRADA_SALIDA_H__
#define __ENTRADA_SALIDA_H__

#include <iostream>
#include <cstring>
#include <cstdlib> //Para usar system
#include <fstream> //Para trabajar con ficheros

#include "grafo.hpp"


using namespace std;



//Carga el grafo desde dos ficheros que contengan la matriz de conexion y las etiquetas de los nodos.
template <class G_Nodo, class G_Lado>
bool cargarGrafo(GrafoDenso<G_Nodo, G_Lado> * &g)
{
	string ficheroMatriz, ficheroEtiquetas;
	
	cout << "Fichero de la matriz de conexion: ";
	cin >> ficheroMatriz;
	
	cout << "Fichero de las etiquetas: ";
	cin >> ficheroEtiquetas;
	
	ifstream f(ficheroMatriz.c_str()); //Se define el stream de lectura y se abre el fichero de la matriz de conexion.
	
	if (not f)
		return false;

	//Se definen variables auxiliares para cargar el grafo.
	unsigned int nodos;
	unsigned int tipo;
	bool dirigido;
	G_Lado peso;
	G_Nodo etiqueta;

	f >> nodos; //Se lee el numero de nodos.
	f >> tipo; //Se lee el tipo.
	
	if (tipo == 1)
		dirigido = true;
	if (tipo == 0)
		dirigido = false;

	g = new GrafoDenso<G_Nodo, G_Lado>(nodos, dirigido); //Se reserva espacio para el grafo y se invoca al constructor.

	//Se carga la matriz de conexion desde el fichero.
	for(unsigned int i = 0; i < g->numeroNodos(); i++)
		for(unsigned int j = 0; j < g->numeroNodos(); j++)
		{
			f >> peso;
			g->asignarPeso(i, j, peso);
		}

	f.close(); // Se cierra el fichero.
	
	ifstream f2(ficheroEtiquetas.c_str()); //Se define el stream de lectura y se abre el fichero de las etiquetas
	
	if (not f2)
	{
		g->borrarGrafo();
		return false;
	}
	
	for(unsigned int i = 0; i < g->numeroNodos(); i++)
	{
		f2 >> etiqueta;
		g->asignarEtiqueta(i, etiqueta);
	}
	
	f2.close();
	return true;
}


#endif

	
	
	
	
	

