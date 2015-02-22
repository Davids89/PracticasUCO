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

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp> // entrada salida de imagenes, hay que descomentarla
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>

/*!\brief Define los parámetro opcionales de tu programa.

  Redefine esta clase en función de tus necesitados
*/
struct CLIParams
{
  CLIParams ()
    : entero(0), //Valores por defecto de los argumentos opcionales.
      flotante(0.0),
      cadena(""),
      verbose(false)
    {}
  int entero;
  float flotante;  
  const char * cadena;
  bool verbose;
};

/*!\brief Muestra la ayuda del programa.
  Redefinir en función de los parámetros utilizados realmente.
  \arg[in] programe es el nombre del programa en la cli.
*/
static void
mostrarUso (const char * progname) throw ()
{
  std::cout << "Esto programa sirve para ...." << std::endl;
  std::cout << "Uso: " << progname << " [-h] [-v] [-i valor] [-c valor] [-f valor] arg1 arg2 ... argn" << std::endl;
  std::cout << "Donde: " << std::endl;
  std::cout << "-h\tMuestra  esta la ayuda." << std::endl;
  std::cout << "-v\tActiva el modo verbose." << std::endl;
  std::cout << "-i\tPermite espacificar un valor entero. Valor por defecto 0." << std::endl;
  std::cout << "-c\tPermite especificar una cadena. Valor por defecto NUL." << std::endl;
  std::cout << "-f\tPermite especificar un valor flotante. Valor por defecto 0.0" << std::endl;
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
  while ((opcion = getopt (argc, argv, "hvi:c:f:")) != -1)
  {
    switch (opcion)
    {
      
      case 'h':
	mostrarUso(argv[0]);
	exit (EXIT_SUCCESS);
	break;
	
      case 'v':
	params.verbose=true;
	break;
	
      case 'i':
	params.entero = atoi (optarg);
	break;

      case 'c':
	params.cadena = optarg;
	break;
	
      case 'f':
	params.flotante = atof(optarg);
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
    std::cout << "-v\t" << ((params.verbose)?"True":"False") << std::endl;
    std::cout << "-i\t" << params.entero << std::endl;
    std::cout << "-f\t" << params.flotante << std::endl;
    std::cout << "-c\t" << '\"' << params.cadena << '\"' << std::endl;

    std::cout << "Hay " << argc-argObligados << " parámetros obligados que son: " << std::endl;

    for (int i = argObligados; i<argc; ++i)
      std::cout << '\"' << argv[i] << '\"' << std::endl;


    /*Ahora toca que tu rellenes con lo que hay que hacer ...*/

    
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
