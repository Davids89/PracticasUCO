#include <iostream>
#include "ImageStats.hpp"
#include <cmath>

using namespace std;
using namespace FSIV;

namespace FSIV{

void ImageStats::processImage (const cv::Mat& img)
{
	int i,j;
	float nElementos;
	double aux1=0.0;
	double aux2=0.0;
	vector <double> mediana; //este vector es para la mediana

	nElementos=img.cols * img.rows;

	//colocamos como maximo y minimos los valores iniciales e iremos comparando

	for(i=0;i<img.rows;i++)
	{
		for(j=0;j<img.cols;j++)
		{

			//lo primero es meter el elemento en el vector para despues sacar la mediana
			mediana.push_back(img.at <unsigned char> (i,j));

			//valor maximo
			if(img.at <unsigned char> (i,j) > getMax())
			{
				setMax(img.at <unsigned char> (i,j));
			}

			//valor minimo
			if(img.at <unsigned char> (i,j) < getMin())
			{
				setMin(img.at <unsigned char> (i,j));
			}

			//suma
			setSum(getSum()+img.at <unsigned char> (i,j));

			//suma cuadrados
			setSqrSum(getSqrSum() + img.at <unsigned char> (i,j) * img.at <unsigned char> (i,j));

			//area positiva
			if(img.at <unsigned char> (i,j) >= 0)
			{
				setPositivo(getPositivo() + img.at <unsigned char>(i,j));
			}

			//area negativa
			if(img.at <unsigned char> (i,j) < 0)
			{
				setNegativo(getNegativo() + img.at <unsigned char>(i,j));
			}

			//numero de ceros
			if(img.at <unsigned char> (i,j) == 0)
			{
				setZeros(getZeros() + 1);
			}

			//coeficiente de asimetria
			//aux1 += pow( ((img.at <unsigned char>(i,j) - getMean())),3);
			//aux2 += pow( ((img.at <unsigned char>(i,j) - getMean())),2);

		}//for j
	}//for ir

	//media
	setMean(getSum() / nElementos);

	for(i=0;i<img.rows;i++)
	{
		for(j=0;j<img.cols;j++)
		{
			//coeficiente de asimetria
			aux1 += pow( ((img.at <unsigned char>(i,j) - getMean())),3);
			aux2 += pow( ((img.at <unsigned char>(i,j) - getMean())),2);
		}
	}

	//varianza
	setVariance( (getSqrSum()/nElementos) - pow( getMean(), 2) );

	//coeficiente de asimetria
	setSkewees( (aux1/nElementos) / pow( (aux2/nElementos), 1.5) );

	//calcular mediana
	sort(mediana.begin(),mediana.end());//ordenamos el vector
	setMedian(mediana[(double)round(mediana.size()/2)]);
	
}// process end

void ImageStats::processMask (const cv::Mat &img, cv::Mat & mask)
{
	int i,j,contador=0;
	double aux1, aux2;

	vector <double> mediana;

	aux1=aux2=0.0;

	for (i=0;i<img.rows;i++)
	{
		for(j=0;j<img.cols;j++)
		{
			if( mask.empty() or mask.at <unsigned char>(i,j)!=0)
			{
				contador++;

				//mediana
				mediana.push_back(img.at <unsigned char> (i,j));

				//valor maximo
				if(img.at <unsigned char>(i,j) > _max)
				{
					setMax(img.at <unsigned char> (i,j));
				}

				//valor minimo
				if(img.at <unsigned char>(i,j) < _min)
				{
					setMin(img.at <unsigned char> (i,j));
				}

				//suma
				setSum(getSum()+img.at <unsigned char> (i,j));

				//suma cuadrados
				setSqrSum(getSqrSum() + img.at <unsigned char> (i,j) * img.at <unsigned char> (i,j));

				//area positiva
				if(img.at <unsigned char> (i,j) >= 0)
				{
					setPositivo(getPositivo() + img.at <unsigned char>(i,j));
				}

				//area negativa
				if(img.at <unsigned char> (i,j) < 0)
				{
					setNegativo(getNegativo() + img.at <unsigned char>(i,j));
				}

				//numero de ceros
				if(img.at <unsigned char> (i,j) == 0)
				{
					setZeros(getZeros() + 1);
				}

				//coeficiente de asimetria
				//aux1 += pow( (img.at <unsigned char> (i,j) - _mean) , 3);
				//aux2 += pow( (img.at <unsigned char> (i,j) - _mean) , 2);
			}// fin if
		}//fin for j
	}//fin for i

	//media
	setMean(getSum() / contador);

	for(i=0;i<img.rows;i++)
	{
		for(j=0;j<img.cols;j++)
		{
			if( mask.empty() or mask.at <unsigned char>(i,j)!=0)
			{
				aux1 += pow( (img.at <unsigned char> (i,j) - getMean()) , 3);
				aux2 += pow( (img.at <unsigned char> (i,j) - getMean()) , 2);
			}
		}
	}

	//varianza
	setVariance( (getSqrSum()/contador) - pow( getMean(), 2) );

	//coeficiente de asimetria
	setSkewees( (aux1/contador) / pow( (aux2/contador), 1.5) );

	//mediana
	sort(mediana.begin(),mediana.end());
	setMedian(mediana[(double)round(mediana.size()/2)]);
}

void ImageStats::print()
{
	cout << "Valor minimo: " << getMin() << endl;
	cout << "Valor maximo: " << getMax() << endl;
	cout << "Valor de la media: " << getMean() << endl;
	cout << "Valor de la varianza: " << getVariance() << endl;
	cout << "Suma: " << getSum() << endl;
	cout << "Suma de cuadrados: " << getSqrSum() << endl;
	cout << "Valor de la mediana: " << getMedian() << endl;
	cout << "Area positiva: " << getPositivo() << endl;
	cout << "Area negativa: " << getNegativo() << endl;
	cout << "Numero de ceros: " << getZeros() << endl;
	cout << "Coeficiente de asimetria: " << getSkewees() << "\n" << endl;
}

}//fin namespace
