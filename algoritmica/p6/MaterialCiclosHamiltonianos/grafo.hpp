#ifndef __GRAFO_DENSO_HPP__
#define __GRAFO_DENSO_HPP__

#include <cassert>

//! Clase grafo

/*!
    Esta es la clase que representa a un grafo, sea este dirigido
    o no dirigido.
    El grafo depende del valor almacenado para cada lado (G_Lado) y de la etiqueta asociada a cada nodo (G_Nodo)
 */

namespace ed
{
	template <class G_Nodo, class G_Lado>
	class GrafoDenso
	{
		public:
			//Constructor vacio.
			GrafoDenso() throw ()
			{
				numeroNodos(0);
				dirigido(false);
				_matriz = _etiqueta = 0;
			}
			
			//Constructor parametrizado
			GrafoDenso( const unsigned int n, const bool direccion = false /*parametro por defecto*/ ) throw ()
			{
				assert (n > 0);
				
				//Completar los siguientes apartados.
				
				//Asignamos el número de nodos

				//Asignamos la posible dirección

				
				//Reservamos espacio para la matriz de conexion y para las etiquetas

			}
			
			//Constructor de copia
			GrafoDenso(const GrafoDenso<G_Nodo, G_Lado> &g)
			{
				numeroNodos(0);
				_matriz = 0;
				_etiqueta = 0;
				*this = g;
			}
			
			//Destructor
			~GrafoDenso()
			{
				if (not estaVacio())
					borrarGrafo();
			}
			
			//Sobrecarga de la asignacion
			GrafoDenso & operator=(const GrafoDenso<G_Nodo, G_Lado> &g) throw()
			{
				if (this != &g) //Se comprueba que el grafo no se copie a si mismo
				{
					if (not estaVacio()) //Se obliga a que el grafo de la izquierda esté vacío
						borrarGrafo();
					if (not g.estaVacio()) //Solo se copia cuando el grafo de la derecha no está vacío
					{
						//Completar los siguientes apartados.
						
						//Copiamos numero de nodos

						//Copiamos si el grafo es dirigido o no dirigido
						
						//Reservamos espacio para la matriz de conexion y para las etiquetas
					 
						//Copiamos la matriz de conexion y las etiquetas

					}
				}
				return *this;
			}
			void borrarGrafo() throw()
			{
				if (not estaVacio())
				{
					//Completar
				}
			}
			
			//Observadores
			const bool estaVacio()const throw()
			{
				return _matriz == 0;
			}
			
			const unsigned int numeroNodos()const throw()
			{
				return _numeroNodos;
			}
			
			const bool dirigido() const throw()
			{
				return _dirigido;
			}
			
			const G_Lado verPeso(const unsigned int i, const unsigned int j) const throw()
			{
				assert(i < numeroNodos());
				assert(j < numeroNodos());
				return _matriz[i][j];
			}
			
			const G_Nodo verEtiqueta(const unsigned int i) const throw()
			{
				assert(i < numeroNodos());
				return _etiqueta[i];
			}
			
			//Modificadores
			void numeroNodos(const unsigned int n) throw()
			{
				_numeroNodos = n;
			}
			
			void dirigido(const bool direccion) throw()
			{
				_dirigido = direccion;
			}
			
			void asignarPeso(const unsigned int i, const unsigned int j, const G_Lado peso) throw()
			{
				assert(i < numeroNodos());
				assert(j < numeroNodos());
				_matriz[i][j] = peso;
			}
			
			void asignarEtiqueta(const unsigned int i, const G_Nodo etiqueta) throw()
			{
				assert(i < numeroNodos());
				_etiqueta[i] = etiqueta;
			}	

		private:
      // Número de nodos del grafo.
      unsigned int _numeroNodos;
			//Direccion del grafo
      bool _dirigido;
      // Matriz de conexiones del grafo.
      G_Lado **_matriz;
			//Vector de etiquetas del grafo
			G_Nodo *_etiqueta;
	};
}


#endif
