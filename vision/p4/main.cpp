#include "levenstein.hpp"
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>
#include <sstream>

//para compilar g++ -Wall -o practica4 ithreshold.cc `pkg-config opencv --cflags --libs`
//para ejecutar en metodo local ./practica4 -i libro3.png -o cosa.bpm
//para ejecutar en metodo local con umbral local ./practica4 -i libro3.png -o cosa.pgm -c 10
//para ejecutar en metodo local con radio ./practica4 -i libro3.png -o cosa.pgm -r algo
//para ejecutar en metodo global ./practica4 -i libro3.png -o cosa.pgm -m 1
//para ejecutar en metodo global con black hat ./practica4 -i libro3.png -o cosa.pgm -m 1 -t 1
//para ejecutar en metodo global con black hat y radio ./practica4 -i libro3.png -o cosa.pgm -m 1

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> // entrada salida de imagenes, hay que descomentarla
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
using namespace std;

struct CLIParams
{
  CLIParams ()
	: metodo(0), //Valores por defecto de los argumentos opcionales.
	  blackhat(0),
	  radio(5),
	  umbral(0.0),
	  imagen(""),
	  destino("")
	{}
  int metodo;
  int blackhat;
  int radio;
  float umbral;
  const char * imagen;
  const char * destino;
};

static int
parseCLI (int argc, char* const* argv, CLIParams& params) throw ()
{
  int opcion;
  while ((opcion = getopt (argc, argv, "m:tr:c:i:d:")) != -1)
  {
	switch (opcion)
	{
	  
	case 'm':
		params.metodo = atoi(optarg);
		break;
	
	case 't':
		params.blackhat = 1;
		break;
	
	case 'r':
		params.radio = atoi(optarg);
		break;

	case 'c':
		params.umbral = atof(optarg);
		break;

	case 'i':
		params.imagen = optarg;
		break;

	case 'd':
		params.destino = optarg;
		break;
	
	}  // case
	
  }// while
  return optind;
}

int main(int argc, char* const* argv)
{
	int codigo=EXIT_SUCCESS;

	CLIParams params;

	int argObligados = parseCLI(argc, argv, params);
	std::cout << "Los parámetros opcioneales son:" << std::endl;
	std::cout << "-m\t" << params.metodo << std::endl;
	std::cout << "-t\t" << params.blackhat << std::endl;
	std::cout << "-r\t" << params.radio << std::endl;
	std::cout << "-c\t" << params.umbral << std::endl;
	std::cout << "-i\t" << params.imagen << std::endl;

	std::cout << "Hay " << argc-argObligados << " parámetros obligados que son: " << std::endl;

	for (int i = argObligados; i<argc; ++i)
		std::cout << '\"' << argv[i] << '\"' << std::endl;

	try
	{

		Mat imagen, imagenFinal; //Declaramos las variables

		cout << params.imagen << endl;

		//Asignamos las imagenes
		imagen = imread(params.imagen, -1);
		if (imagen.empty())
		{
			cout << "Error al cargar la imagen" << endl;
		}

		imagenFinal = imread(params.imagen, -1);
		if (imagenFinal.empty())
		{
			cout << "Error al cargar imagenFinal" << endl;
		}

		int radio = params.radio;
		float umbral = params.umbral;

		imshow("Antes",imagen);
		if(params.metodo == 0)//Local (Niblack)
		{
			adaptiveThreshold(imagen, imagenFinal, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, radio, umbral);
		}
		else//Global
		{
			if(params.blackhat == 0)//Global sin blackhat
			{
				threshold(imagen, imagenFinal, 0,255,THRESH_OTSU | THRESH_BINARY);
			}
			else//Global con blackhat
			{
				int morph_elem = 0;

				Mat element = getStructuringElement( morph_elem, Size( 2*radio + 1, 2*radio+1 ), Point( radio, radio ) );
				morphologyEx(imagen, imagen, 6, element);	
				threshold(imagen, imagenFinal, 0,255,THRESH_OTSU | THRESH_BINARY);
			}
		}
		imshow("Despues",imagenFinal);
		imwrite(params.destino,imagenFinal);
		waitKey(0);

		//ocrad destino.pgm -o v1.txt --cut 74,94,615,189
		//ocrad destino.pgm -o v2.txt --cut 350,279,615,404
		//ocrad destino.pgm -o v3.txt --cut 232,504,424,942
		//ocrad destino.pgm -o v4.txt --cut 80,915,615,1074

		ifstream fich1("texto1.txt"),fich2("v1.txt");
		ifstream fich3("texto2.txt"),fich4("v2.txt");
		ifstream fich5("texto3.txt"),fich6("v3.txt");
		ifstream fich7("texto4.txt"),fich8("v4.txt");
		
		stringstream buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7, buffer8 ;

		buffer1 << fich1.rdbuf();
		buffer2 << fich2.rdbuf();
		buffer3 << fich3.rdbuf();
		buffer4 << fich4.rdbuf();
		buffer5 << fich5.rdbuf();
		buffer6 << fich6.rdbuf();
		buffer7 << fich7.rdbuf();
		buffer8 << fich8.rdbuf();
		
		fich1.close();
		fich2.close();
		fich3.close();
		fich4.close();
		fich5.close();
		fich6.close();
		fich7.close();
		fich8.close();

		string fichero1 = buffer1.str();
		string fichero2 = buffer2.str();
		string fichero3 = buffer3.str();
		string fichero4 = buffer4.str();
		string fichero5 = buffer5.str();
		string fichero6 = buffer6.str();
		string fichero7 = buffer7.str();
		string fichero8 = buffer8.str();

   		//Distancia Levenshtein
		cout << "Distancia levenshtein V1: " << levenshtein(fichero1,fichero2) << endl;
		cout << "Distancia levenshtein V2: " << levenshtein(fichero3,fichero4) << endl;
		cout << "Distancia levenshtein V3: " << levenshtein(fichero5,fichero6) << endl;
		cout << "Distancia levenshtein V4: " << levenshtein(fichero7,fichero8) << endl;

		
	}
	catch(std::exception& e)
	{
		std::cerr << "Capturada excepcion: " << e.what() << std::endl;
		codigo = EXIT_FAILURE;
	}
  
	return codigo;
}