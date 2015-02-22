#ifndef IMAGESTATS_H
#define IMAGESTATS_H

//para mostrar la foto es imshow
//waitkey hace que se quede parada sino la ventana no sale

//at<vec3b>(i,j) para recorrer la matriz y se indexa con [0,1,2] para indicar el canal

//hacer un histograma. Es un vector de 0 a 255 y vamos sumando en cada posicion segun sea el pixel. Por ejemplo si el pixel [2,2] es 255 pues se suma 1 a 255

//usamos split para desentrelazar los canales y trabajar con un canal

#include <opencv2/core/core.hpp>

using namespace cv;

namespace FSIV{

class ImageStats
{
	public:
		/*CONSTRUCTORES*/
		inline ImageStats()
		{		
			setSize(Size(0,0));
			setChannels(0);
			setMask(getMask().zeros(_size,CV_8U));
			setMean(0.0);
			setMedian(0.0);
			setMin(256.0);
			setMax(0.0);
			setVariance(0.0);
			setSum(0.0);
			setSqrSum(0.0);
			setZeros(0.0);
			setSkewees(0.0);
			setNegativo(0.0);
			setPositivo(0.0);			
		}

		/*inline ImageStats(const cv::Size& size, int channels)
		{
			_size=size;
			_channels=channels;
			_mask.zeros(_size,CV_8U);
		}*/

		//Observers
		inline double getMean() const
		{
			return _mean;
		}

		inline double getMedian() const
		{
			return _median;
		}

		inline double getMin() const
		{
			return _min;
		}

		inline double getMax() const
		{
			return _max;
		}

		inline double getVariance() const
		{
			return _variance;
		}

		inline double getSum() const
		{
			return _sum;
		}

		inline double getSqrSum() const
		{
			return _sqrSum;
		}

		inline double getSkewees() const
		{
			return _skewees;
		}

		inline double getZeros() const
		{
			return _zeros;
		}

		inline double getPositivo() const
		{
			return _areaPositiva;
		}

		inline double getNegativo() const
		{
			return _areaNegativa;
		}

		//Modifiers
		inline void setMean(double const &media)
		{
			_mean = media;
		}

		inline void setMedian(double const &mediana)
		{
			_median = mediana;
		}

		inline void setMin(double const &minimo)
		{
			_min = minimo;
		}


		inline void setMax(double const &maximo)
		{
			_max = maximo;
		}

		inline void setVariance(double const &varianza)
		{
			_variance = varianza;
		}

		inline void setSum(double const &suma)
		{
			_sum = suma;
		}

		inline void setSqrSum(double const &sumaCuadrada)
		{
			_sqrSum = sumaCuadrada;
		}

		inline void setZeros(double const &cero)
		{_mask.zeros(_size,CV_8U);
			_zeros = cero;
		}

		inline void setSkewees(double const &coeficiente)
		{
			_skewees = coeficiente;
		}

		inline void setPositivo(const double &positivo)
		{
			_areaPositiva = positivo;
		}
		
		inline void setNegativo(double const &negativo)
		{
			_areaNegativa = negativo;
		}

		//Other functions
		inline void initialize (const cv::Size& s, int channels)
		{
			setSize(Size(0,0));
			setChannels(0);
			setMask(getMask().zeros(_size,CV_8U));
			setMean(0.0);
			setMedian(0.0);
			setMin(256.0);
			setMax(0.0);
			setVariance(0.0);
			setSum(0.0);
			setSqrSum(0.0);
			setZeros(0.0);
			setSkewees(0.0);
			setNegativo(0.0);
			setPositivo(0.0);
		}

		void processImage (const cv::Mat& img); //procesa una imagen para obtener todos sus estadísticos.
		void processMask (const cv::Mat &img, cv::Mat & mask);
		void print();

		//Another observers
		inline cv::Size size() const
		{
			return _size;
		}
		inline int channels() const
		{
			return _channels;
		}
		inline const cv::Mat& getMask() const
		{
			return _mask;
		}

		//Another modifiers
		void setSize(const Size &s)
		{
			_size=s;
		}

		void setChannels(const int &cha)
		{
			_channels=cha;
		}

		inline void setMask(const Mat& m) //pone la mascara, por defecto no hay.
		{
			_mask=m;
		}

	private:
		cv::Size _size;
		int _channels;//channel
		cv::Mat _mask;//opcional
		double _mean;
		double _median;
		double _min;
		double _max;
		double _variance;
		double _sum;
		double _sqrSum;
		double _areaPositiva;
		double _areaNegativa;
		double _zeros;
		double _skewees;
};

}//fin namespace

#endif
