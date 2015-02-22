#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include <cstdio>
#include <cassert>
#include <time.h>
#include <stdlib.h>

using namespace std;

namespace algoritmica
{

template <class T>
class Matriz
{
  public:
		Matriz(const int &f, const int &c)
		{
			filas(f);
			columnas(c);
		
			reservarMemoria();

			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
				   elemento(i, j, 0);
				}
			}  
		}

		Matriz()
		{
			_m = NULL;
			filas(0);
			columnas(0);
		}

		//Constructor de copia.
		Matriz(const Matriz &m)
		{
			filas(m.filas());
			columnas(m.columnas());

			reservarMemoria();
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
				   elemento(i, j, m.elemento(i, j));
				}
			}  
		}

		~Matriz()
		{
			liberarMatriz();
		}

		void liberarMatriz()
		{
			for(int i = 0; i < filas(); ++i)
			{
				delete [] _m[i];
			}

			delete [] _m;
		}

		inline int filas() const
		{
			return _f;
		}

		inline int columnas() const
		{
			return _c;
		}

		inline void filas(const int &f)
		{
			_f = f;
		}

		inline void columnas(const int &c)
		{
			_c = c;
		}

		inline T elemento(const int &f, const int &c) const
		{
			return _m[f - 1][c - 1];
		}

		inline void elemento(const int &f, const int &c, const T &x)
		{
			_m[f - 1][c - 1] = x;
		}

		void verMatriz() const
		{
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
					cout << elemento(i, j) << " ";
				}
				cout << "\n";
			}
		}

		/*void rellenarMatriz()
		{
			T x;
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1; j <= columnas(); j++)
				{
				  cout << " elemento(" << i << "," << j << "): ";
				  cin >> x;
				  elemento(i, j, x);
				}
			}
		}*/

		void rellenarMatriz(const int &inf, const int &sup)
		{
			srand(time(NULL));
			double el;

			for(int i=1; i<=filas(); i++)
			{
				for(int j=1; j<=columnas(); j++)
				{
					el=rand()%(sup-inf+1);
					elemento(i,j,(inf+el));
				}
			}
		}

		//Esta función devuelve la matriz resultante de eliminar la fila f y la columna c de la matriz m
		Matriz eliminarFilaColumna(int f, int c)
		{
			assert((f >= 1 && f <= filas()) && c >= 1 && c <= columnas());
						
			//Se elimina la fila
			Matriz r1 = eliminarFila(f);
			
			//Se elimina la columna
			Matriz resultado = r1.eliminarColumna(c);
			
			return resultado;
		}
		
		//Esta funcion devuelve la matriz resultante al eliminar la fila f
		Matriz eliminarFila(int f)
		{
			assert(f >= 1 && f <= filas());
			Matriz resultado(filas() - 1, columnas());
			//El elemento es igual que su homólogo en filas < f
			for(int i = 1; i < f; i++)
			{
				for(int j = 1 ; j <= columnas(); j++)
				{
					resultado.elemento(i, j, elemento(i,j));
				}
			}
			//las filas > f las retrocedemos una fila
			for(int i = f ; i <= resultado.filas(); i++)
			{
				for(int j = 1 ; j <= columnas(); j++)
				{
					resultado.elemento(i, j, elemento(i + 1,j));
				}
			}
		
			return resultado;
		}
		
		//Esta funcion devuelve la matriz resultante de eliminar la columna c
		Matriz eliminarColumna(int c)
		{
			assert(c >= 1 && c <= columnas());
			Matriz resultado(filas(), columnas() - 1);
			//El elemento es igual que su homólogo en columnas < c
			for(int i = 1; i <= filas(); i++)
			{
				for(int j = 1 ; j < c; j++)
				{
					resultado.elemento(i, j, elemento(i,j));
				}
			}
			//las columnas > c las retrocedemos una columna
			for(int i = 1 ; i <= resultado.filas(); i++)
			{
				for(int j = c ; j <= resultado.columnas(); j++)
				{
					resultado.elemento(i, j, elemento(i, j + 1));
				}
			}
				
			return resultado;
		}

			
		bool vacia()const
		{
			if (_m == NULL)
				return true;
			return false;
		}

		Matriz& operator=(const Matriz &m2)
		{
			filas(m2.filas());
			columnas(m2.columnas());
		
			if (vacia()) //Se comprueba si la matriz del primer miembro esta vacia.
			{
				reservarMemoria();
				for(int i = 1; i <= filas(); i++)
				{
				  for(int j = 1; j <= columnas(); j++)
				  {
				    elemento(i, j, m2.elemento(i, j));
				  }
				}
			}
			else //La matriz del primer miembro ya estaba creada.
			{
				assert (filas() == m2.filas() && columnas() == m2.columnas());
				//Al tener las mismas dimensiones se pueden asignar.
				for(int i = 1; i <= filas(); i++)
				{
				  for(int j = 1; j <= columnas(); j++)
				  {
				    elemento(i, j, m2.elemento(i, j));
				  }
				}
			}

			return *this;
		}
		 
  private:
    T **_m;
    int _f;
    int _c;
    void reservarMemoria()
		{
			_m = new T * [filas()];

			for(int i = 0; i < filas(); ++i)
				_m[i] = new T [columnas()];
		}
};

}//fin namespace algoritmica


#endif
