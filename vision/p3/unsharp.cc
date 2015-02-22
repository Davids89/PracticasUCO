//g++ -Wall -o esqueleto esqueleto.cc `pkg-config opencv --cflags --libs`

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <exception>
#include <vector>
#include <cmath>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> // entrada salida de imagenes, hay que descomentarla
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

/*!\brief Define los parámetro opcionales de tu programa.

  Redefine esta clase en función de tus necesitados
*/
struct CLIParams
{
  CLIParams ()
    : frecuencia(0.5), //Valores por defecto de los argumentos opcionales.
	orden(2),
	ganancia(1.0),
	imagen(""),
	mascara(""),
	interactivo(0)
    {}
	float frecuencia; 
	int orden;
	float ganancia;
	const char * imagen;
	const char * mascara;
	int interactivo;
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

static int
parseCLI (int argc, char* const* argv, CLIParams& params) throw ()
{
  // Esta es una forma habitual de recoger argumentos con getopt
  // se usa una iteracion y cada elemento se pasa por un switch-case
  int opcion;
  while ((opcion = getopt (argc, argv, "him:g:r:n:o:")) != -1)
  {
    switch (opcion)
    {
      
      case 'h':
		mostrarUso(argv[0]);
		exit (EXIT_SUCCESS);
		break;

	case 'r': //frecuencia
		params.frecuencia=atof(optarg);
		break;

	case 'g': //ganancia
		params.ganancia=atof(optarg);
		break;
	
	case 'i': //interactivo
		params.interactivo=1;
		break;

	case 'o':
		params.imagen=optarg;
		break;

	case 'm': //mascara
		params.mascara=optarg;
		break;

	case 'n': //orden
		params.orden=atoi(optarg);
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

using namespace cv;
using namespace std;

void create_butterworth_lowpass_filter(Mat &imagen, float D, int n);
void realizarFiltro(Mat &imagen, CLIParams &params, Mat &destino, Mat &mascara);
void ver(Mat &imagen, Mat &resultado);
void centrarDFT(Mat &imagen);
void ponerMascara(Mat &imagen, Mat &filtrada, Mat &mascara, Mat &destino);
void butterworth(const Mat &img , float r, int n, Mat &imgButter);

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
    std::cout << "-r\t" << params.frecuencia << std::endl;
    std::cout << "-n\t" << params.orden << std::endl;
    std::cout << "-g\t" << params.ganancia << std::endl;

    std::cout << "Hay " << argc-argObligados << " parámetros obligados que son: " << std::endl;

    for (int i = argObligados; i<argc; ++i)
      std::cout << '\"' << argv[i] << '\"' << std::endl;

	Mat imagen, mascara, auxImagen, destino, auxDestino, filtro; //imagenes
	vector <Mat> v;//vector de imagenes

	imagen=imread(params.imagen,-1);
	if(imagen.empty())
		cout << "Imagen no cargada" << endl;

	destino=imread(params.imagen,0);
	if(destino.empty())
		cout << "Destino no cargada" << endl;

	if(imagen.channels()==1)
	{

		mascara=imread(params.mascara,0);
		if(imagen.empty())
			cout << "Mascara no cargada" << endl;
		//Imagen monocroma

		if(params.interactivo == 0)
		{
			realizarFiltro(imagen, params, destino, mascara);

			if(!mascara.empty())
			{
				imagen.convertTo(imagen, CV_32F, 1/255.0, 0);
				mascara.convertTo(mascara, CV_32F, 1/255.0, 0);
				destino.convertTo(destino, CV_32F, 1/255.0, 0);

				destino = destino.mul(mascara) + imagen.mul(1 - mascara);

				normalize(destino, destino, 0, 1, CV_MINMAX);
				destino.convertTo(destino, CV_8U, 255, 0);
			}
			imshow("Imagen filtrada", destino);
			imshow("Original", imagen);
			waitKey(0);
		}
		else
		{
			//Crea la ventana
			namedWindow("Unsharp mask", 1);
			//Modificar ganancia
			int Slider1 = params.ganancia;
			createTrackbar("Ganancia", "Unsharp mask", &Slider1, 5);
			//Modificar orden
			int Slider2 = params.orden;
			createTrackbar("Filtro", "Unsharp mask", &Slider2, 10);
			//Modificar frecuencia
			int Slider3 =  params.frecuencia;
			createTrackbar("Frecuencia", "Unsharp mask", &Slider3, 1);
			while (true)
			{

				CLIParams paramsAux;

				paramsAux.ganancia = Slider1;
				paramsAux.frecuencia = Slider3;
				paramsAux.orden = Slider2;

				realizarFiltro(imagen, paramsAux, destino, mascara);

				if(!mascara.empty())
				{
					imagen.convertTo(imagen, CV_32F, 1/255.0, 0);
					mascara.convertTo(mascara, CV_32F, 1/255.0, 0);
					destino.convertTo(destino, CV_32F, 1/255.0, 0);

					destino = destino.mul(mascara) + imagen.mul(1 - mascara);

					normalize(destino, destino, 0, 1, CV_MINMAX);
					destino.convertTo(destino, CV_8U, 255, 0);
				}

				imshow("Unsharp mask",destino);
				
				char c = waitKey(10);
					if (c == 27 or c == '\r')
					{
						break;
					}
			}
			//Destruye la ventana "Unsharp"
			destroyWindow("Unsharp");
		}
	}
	else
	{
		//Imagen a color

		mascara=imread(params.mascara,0);
		if(imagen.empty())
			cout << "Mascara no cargada" << endl;

		if(params.interactivo == 0)
		{
			cvtColor(imagen, auxImagen, CV_BGR2HSV);
			vector <Mat> v, v2;//declaración del vector para los canales
			split(auxImagen,v);
			Mat auxiliar = v[2];
			Mat auxiliar2;
			realizarFiltro(auxiliar, params, v[2], mascara);
			imshow("Gris", auxiliar);

			/*if(!mascara.empty())
			{
				cout << "Imagen" << v[2].size() << " canales" << v[2].channels() << endl;
				cout << "Mascara" << mascara.size() << " canales" << mascara.channels() << endl;
				cout << "Destino" << destino.size() << " canales" << destino.channels() << endl;
				cout << "Auxiliar" << auxiliar.size() << " canales" << auxiliar.channels() << endl;
				imagen.convertTo(imagen, CV_32F, 1/255.0, 0);
				mascara.convertTo(mascara, CV_32F, 1/255.0, 0);
				//cvtColor(mascara, mascara, CV_BGR2HSV);
				destino.convertTo(destino, CV_32F, 1/255.0, 0);
				v[2].convertTo(v[2], CV_32F, 1/255.0, 0);

				destino = v[2].mul(mascara) + imagen.mul(1 - mascara);

				normalize(destino, destino, 0, 1, CV_MINMAX);
				destino.convertTo(destino, CV_8U, 255, 0);
			}*/

			merge(v,destino);

			if(!mascara.empty())
			{
				imagen.convertTo(imagen, CV_32F, 1/255.0, 0);
				mascara.convertTo(mascara, CV_32F, 1/255.0, 0);
				destino.convertTo(destino, CV_32F, 1/255.0, 0);

				split(imagen, v);
				split(destino, v2);

				v2[2] = v2[2].mul(mascara) + v[2].mul(1 - mascara);
				merge(v2, destino);

				normalize(destino, destino, 0, 1, CV_MINMAX);
				destino.convertTo(destino, CV_8U, 255, 0);
			}

			cvtColor(destino, destino, CV_HSV2BGR);
			imshow("Imagen filtrada", destino);
			imshow("Original", imagen);
			waitKey(0);
		}
		else
		{
			//Crea la ventana
			namedWindow("Unsharp mask", 1);
			//Modificar ganancia
			int Slider1 = params.ganancia;
			createTrackbar("Ganancia", "Unsharp mask", &Slider1, 5);
			//Modificar orden
			int Slider2 = params.orden;
			createTrackbar("Filtro", "Unsharp mask", &Slider2, 10);
			//Modificar frecuencia
			int Slider3 =  params.frecuencia;
			createTrackbar("Frecuencia", "Unsharp mask", &Slider3, 1);
			while (true)
			{

				CLIParams paramsAux;

				paramsAux.ganancia = Slider1;
				paramsAux.frecuencia = Slider3;
				paramsAux.orden = Slider2;

				cvtColor(imagen, auxImagen, CV_BGR2HSV);
				vector <Mat> v;//declaración del vector para los canales
				split(auxImagen,v);
				realizarFiltro(v[2], paramsAux, v[2], mascara);
				merge(v, destino);
				cvtColor(destino, destino, CV_HSV2BGR);
				imshow("Unsharp mask",destino);
				
				char c = waitKey(10);
					if (c == 27 or c == '\r')
					{
						break;
					}
			}
			//Destruye la ventana "Unsharp"
			destroyWindow("Unsharp");
		}
	}
    
  }
  catch (std::exception& e)
  {
    std::cerr << "Capturada excepcion: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}

void realizarFiltro(Mat &imagen, CLIParams &params, Mat &destino, Mat &mascara)
{
	vector <Mat> v;

	Mat auxImagen, auxImagen2, auxDestino, fourier, muestra;

	imagen.convertTo(auxImagen,CV_32F, 1/255.0, 0); //convertimos la imagen a flotante

	dft(auxImagen, fourier, DFT_COMPLEX_OUTPUT); //hacemos la transformada

	Mat filter = imagen.clone(); //clonamos para las operaciones

	create_butterworth_lowpass_filter(filter, params.frecuencia, params.orden); //hacemos el filtro

	//butterworth(imagen, params.frecuencia, params.orden, filter);

	auxImagen2=imagen.clone();

	mulSpectrums(fourier, filter, auxImagen2, 0); //multiplicamos la imagen por el filtro

	ver(auxImagen2, muestra);
	imshow("MULTIPLICACION", muestra);

	ver(fourier, muestra);
	imshow("FOURIER", muestra);

	dft(auxImagen2, auxImagen2, DFT_SCALE + DFT_INVERSE + DFT_REAL_OUTPUT); //hacemos la dft inversa

	normalize(auxImagen2, auxImagen2, 0, 1, CV_MINMAX);

	auxImagen2.convertTo(auxImagen2, CV_8U, 255, 0);

	destino = (params.ganancia +1.0) * imagen - (params.ganancia * auxImagen2);

}

void create_butterworth_lowpass_filter(Mat &imagen, float D, int n)
{
    Mat tmp = Mat(imagen.rows, imagen.cols, CV_32F);

    Point centre = Point(imagen.rows / 2, imagen.cols / 2);
    double radius;

    D = sqrt((pow(imagen.rows / 2, 2)) + (pow(imagen.cols / 2, 2))) * D;

    for (int i = 0; i < imagen.rows; i++)
    {
        for (int j = 0; j < imagen.cols; j++)
        {
            radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
            tmp.at<float>(i, j) = (float)( 1 / (1 + pow((double) (radius / D), (float) (2 * n))));
        }
    }
    
    Mat tmp2;
    normalize(tmp, tmp2, 0, 255, CV_MINMAX);
    imshow("Filtro", tmp);

    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, imagen);
}

void butterworth(const Mat &img , float r, int n, Mat &imgButter)
{

	Mat tmp = Mat(img.rows, img.cols, CV_32F);

	Point centre = Point(img.rows / 2, img.cols / 2);
	double radius;

	r=sqrt(pow(img.rows/2,2)+ pow(img.cols/2,2))*r;
	for (int i=0; i<img.rows; i++)
	{
		for (int j=0; j<img.cols; j++)
		{	
			//pixel=img.at <unsigned char> (i, j);
			//if (Mask.empty() || Mask.at <unsigned char> (i, j) != 0 )
			//{
				//imgButter.at <unsigned char> (i, j)=1/(1+pow(((pixel)/r),2*n));
				radius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));
				imgButter.at<float>(i,j) = (float)( 1 / (1 + pow((double) (radius/r), (double) (2 * n))));
			//}	
		}   
 
	}
		tmp=imgButter;
		tmp.convertTo(tmp,CV_32F, 255.0, 0);
    	normalize(tmp, tmp, 0, 255, CV_MINMAX);
    	imshow("Filtro", tmp);
		
		Mat toMerge[] = {tmp, tmp};
		merge(toMerge, 2, imgButter);
		//imshow("Imagen Butterworth", imgButter);

}

void centrarDFT(Mat &imagen )
{
    Mat tmp, q0, q1, q2, q3;

    imagen = imagen(Rect(0, 0, imagen.cols & -2, imagen.rows & -2));

    int cx = imagen.cols / 2;
    int cy = imagen.rows / 2;

    q0 = imagen(Rect(0, 0, cx, cy));
    q1 = imagen(Rect(cx, 0, cx, cy));
    q2 = imagen(Rect(0, cy, cx, cy));
    q3 = imagen(Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void ver(Mat &imagen, Mat &resultado)
{

	vector <Mat> planes; //vector para las matrices real e imaginaria

	split(imagen, planes); //dividimos la imagen en real e imaginaria

	magnitude(planes[0], planes[1], planes[0]); //magnificamos

	Mat mag= planes[0];

	mag += Scalar::all(1); //sumamos escalar
	log(mag, mag);

	centrarDFT(mag);

	normalize(mag, mag, 0, 1, CV_MINMAX);

	resultado = mag.clone();
}

void ponerMascara(Mat &imagen, Mat &filtrada, Mat &mascara, Mat &destino)
{
	Mat aux11, aux12, aux2;

	vector <Mat> v;

	split(filtrada, v);

	aux11 = v[0] * mascara;
	aux12 = v[1] * mascara;

	merge(v, filtrada);

	aux2 = 1 - imagen;

	destino = filtrada + aux2;
}