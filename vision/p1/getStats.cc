/*!
  Esto es un esqueleto de programa para usar en las prácticas
  de Visión Artificial.

  Se supone que se utilizará OpenCV.

  Para compilar, puedes ejecutar:
    g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

*/

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>
#include "ImageStats.hpp"

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> // entrada salida de imagenes, hay que descomentarla
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

/*!\brief Define los parámetro opcionales de tu programa.

  Redefine esta clase en función de tus necesitados
*/

using namespace FSIV; //para usar la clase y los metodos creados
using namespace std;
using namespace cv;

struct CLIParams
{
  CLIParams (): //Valores por defecto de los argumentos opcionales.
      mascara(""),
	imagen("")
    {}
	const char * mascara;
	const char * imagen;
};

/*!\brief Muestra la ayuda del programa.
  Redefinir en función de los parámetros utilizados realmente.
  \arg[in] programe es el nombre del programa en la cli.
*/
static void
mostrarUso (const char * progname) throw ()
{
	cout << "---------------------------------------------------------------" << endl;
  std::cout << "Este programa sirve para calcular estadisticos de imagenes" << std::endl;
	cout << "--------------------------------------------------------------- \n" << endl;
  std::cout << "Uso: " << progname << " [-h] [-i valor] [-m valor] arg1 arg2 ... argn" << std::endl;
  std::cout << "Donde: " << std::endl;
  std::cout << "-h\tMuestra  esta la ayuda." << std::endl;
  std::cout << "-i\tNombre de la imagen." << std::endl;
  std::cout << "-m\tNombre de la mascara" << std::endl;
}

/*!\brief Parsea la linea de comandos.
  \arg[in] argc es el número de argumentos en la línea.
  \arg[in] argv son los argumentos de la línea.
  \arg[out] params son los parámetros opcionales parseados.
  \return El índice del primer argumento no opcional de la línea.
  \warning Esta función no retorna si hay algún error en la cli.
*/
static int
parseCLI (int argc, char* const* argv, CLIParams& params) throw ()
{
  // Esta es una forma habitual de recoger argumentos con getopt
  // se usa una iteracion y cada elemento se pasa por un switch-case
  int opcion;
  while ((opcion = getopt (argc, argv, "hi:m:")) != -1)
  {
    switch (opcion)
    {
      
      case 'h':
	mostrarUso(argv[0]);
	exit (EXIT_SUCCESS);
	break;
	
      /*case 'v':
	params.verbose=true;
	break;*/
	
      case 'i':
	params.imagen = optarg;
	break;

      case 'm':
	params.mascara = optarg;
	break;
	
	
      case '?': // en caso de error getopt devuelve el caracter ?
	
	if (isprint (optopt))
	  std::cerr << "Error: Opción desconocida \'" << optopt
	    << "\'" << std::endl;
	else
	  std::cerr << "Error: Caracter de opcion desconocido \'x" << std::hex << optopt
	    << "\'" << std::endl;
	mostrarUso(argv[0]);    
	exit (EXIT_FAILURE);
	
	// en cualquier otro caso lo consideramos error grave y salimos
      default:
	std::cerr << "Error: línea de comandos errónea." << std::endl;
	mostrarUso(argv[0]);
	exit(EXIT_FAILURE);	
    }  // case
    
  }// while
  return optind;
}



int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;
  
  try {    
    CLIParams params;

    int argObligados = parseCLI(argc, argv, params);

    std::cout << "Los parámetros opcioneales son:" << std::endl;
    std::cout << "-i\t" << params.imagen << std::endl;
    std::cout << "-m\t" << params.mascara << std::endl;

    std::cout << "Hay " << argc-argObligados << " parámetros obligados que son: " << std::endl;

    for (int i = argObligados; i<argc; ++i)
      std::cout << '\"' << argv[i] << '\"' << std::endl;

	/*Ahora toca que tu rellenes con lo que hay que hacer ...*/

	Mat imagen, mascara; //una matriz para la imagen y otra para la mascara
	vector <Mat> v1;
	vector <Mat> v2;

	imagen=imread(params.imagen,-1);
  	if (imagen.empty())
	{
		cout << "Imagen no cargada" << endl;
	}
	//imshow("Imagen",img);
	//waitKey(0);

	ImageStats stad; //objeto para las estadisticas

	//ahora dividimos la imagen
	split(imagen,v1);//dividimos y la guardamos en v

	//cout << "Nombre de la imagen: " << argv[2] << endl;
	cout << "Tamaño de canales: " << imagen.channels() << endl;
	cout << "Tamaño de la imagen: " << imagen.size() << endl;
	
	//sin mascara
	cout << "Canal 1" << endl;
	cout << "------- \n"<< endl;
	stad.initialize(imagen.size(),1);
	stad.processImage(v1[0]);
	stad.print();

	if(imagen.channels() == 3)
	{
		ImageStats stad2, stad3; //si hay tres canales hay que hacer lo mismo otra dos veces
		cout << "" << endl;
		cout << "Canal 2" << endl;
		cout << "------- \n"<< endl;
		stad2.initialize(imagen.size(),1);
		stad2.processImage(v1[1]);
		stad2.print();

		cout << "" << endl;
		cout << "Canal 3" << endl;
		cout << "------- \n"<< endl;
		stad3.initialize(imagen.size(),1);
		stad3.processImage(v1[2]);
		stad3.print();
	}//fin if

	//con mascara
	if(argc > 3) // quiere decir que hemos metido mascara
	{
		mascara = imread (params.mascara,-1);
		split(mascara, v2);
		cout << "------------------" << endl;
		cout << "Imagen con mascara" << endl;
		cout << "------------------ \n" << endl;
		cout << "Canal 1" << endl;
		cout << "------- \n"<< endl;
		stad.initialize(imagen.size(),1);
		stad.processMask(v1[0],v2[0]);
		stad.print();

		if(imagen.channels() == 3)
		{
			ImageStats stad2, stad3;
			
			cout << "" << endl;
			cout << "Canal 2" << endl;
			cout << "------- \n"<< endl;
			stad2.initialize(imagen.size(),1);
			stad2.processMask(v1[1],v2[1]);
			stad2.print();

			cout << "" << endl;
			cout << "Canal 3" << endl;
			cout << "------- \n"<< endl;
			stad3.initialize(imagen.size(),1);
			stad3.processMask(v1[2],v2[2]);
			stad3.print();
		}//fin if	
	}// fin if argc
    
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
