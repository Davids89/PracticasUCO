#include <math.h>
#include <stdlib.h>
#include <errno.h>
/*  Referencia a la variable global que indica los codigos
   de los errores generados por las funciones matematicas */
extern int errno;

/* Prototipo de la funcion que comprueba errores matematicos **/
double errcheck(double d, char *s);


/*****************************************************************/
/** Nombre: Log                                                 **/
/** Labor que desarrolla: calcula el logaritmo neperiano de un  **/
/**       numero real, pero comprueba antes si hay algun error  **/
/**       en el argumento.                                      **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametro:                                                  **/
/**  Nombre: x                                                  **/
/**    	Tipo: real de doble precision "double"                  **/
/**     Mision: indica el numero al cual se le va aplicar       **/
/**            la funcion logaritmo neperiano                   **/
/*****************************************************************/
double Log(double x)
{
 return errcheck(log(x),"logaritmo neperiano");
 }

/*****************************************************************/
/** Nombre: Log10                                               **/
/** Labor que desarrolla: calcula el logaritmo decimal de un    **/
/**       numero real, pero comprueba antes si hay algun error  **/
/**       en el argumento.                                      **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametro:                                                  **/
/**  Nombre: x                                                  **/
/**    	Tipo: real de doble precision "double"                  **/
/**     Mision: indica el numero al cual se le va aplicar       **/
/**            la funcion logaritmo decimal                     **/
/*****************************************************************/
double Log10(double x)
{
 return errcheck(log10(x),"logaritmo decimal");
}

/*****************************************************************/
/** Nombre: Exp                                                 **/
/** Labor que desarrolla: calcula la exponencial de un numero   **/
/**       real, pero comprueba antes si hay algun error en el   **/
/**       argumento.                                            **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametro:                                                  **/
/**  Nombre: x                                                  **/
/**    	Tipo: real de doble precision "double"                  **/
/**     Mision: indica el numero al cual se le va aplicar       **/
/**            la funcion exponencial                           **/
/*****************************************************************/
double Exp(double x)
{
 return errcheck(exp(x),"exponencial");
}

/*****************************************************************/
/** Nombre: Sqrt                                                **/
/** Labor que desarrolla: calcula la raiz cuadrada de un numero **/
/**       real, pero comprueba antes si hay algun error en el   **/
/**       argumento.                                            **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametro:                                                  **/
/**  Nombre: x                                                  **/
/**    	Tipo: real de doble precision "double"                  **/
/**     Mision: indica el numero al cual se le va aplicar       **/
/**            la funcion raiz cuadrada.                        **/
/*****************************************************************/
double Sqrt(double x)
{
 return errcheck(sqrt(x),"raiz cuadrada");
}

/*****************************************************************/
/** Nombre: integer                                             **/
/** Labor que desarrolla: calcula la parte entera de un numero  **/
/**       real, pero comprueba antes si hay algun error en el   **/
/**       argumento.                                            **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametro:                                                  **/
/**  Nombre: x                                                  **/
/**    	Tipo: real de doble precision "double"                  **/
/**     Mision: indica el numero al cual se le va aplicar       **/
/**            calcular la parte entera.                        **/
/*****************************************************************/

double integer(double x)
{
 return  (double) (long) x;
}

/*****************************************************************/
/** Nombre: Azar                                                **/
/** Labor que desarrolla: calcula un numero aleatorio           **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametros:   ninguno                                       **/
/*****************************************************************/
double Azar()
{
 return (double) (long) rand() / RAND_MAX;
}

/*******************************************************************/
/** Nombre: Atan2                                                 **/
/** Labor que desarrolla: calcula el arco tangente doble, pero    **/
/**       comprueba antes si hay algun error en los argumentos    **/
/** Tipo de resultado: real de doble precision "double"           **/
/** Parametro:                                                    **/
/**  Nombre: x                                                    **/
/**    	Tipo: real de doble precision "double"                    **/
/**     Mision: indica el numerador de la funcion Arco tangente   **/
/**  Nombre: y                                                    **/
/**    	Tipo: real de doble precision "double"                    **/
/**     Mision: indica el denominador de la funcion Arco tangente **/
/*******************************************************************/

double Atan2(double x, double y)
{
 return errcheck(atan(x/y),"arco tangente doble");
}


/*****************************************************************/
/** Nombre: errcheck                                            **/
/** Labor que desarrolla: comprueba si se ha producido un error **/
/**       al realizar la evaluacion matematica del primer       **/
/**       parametro. Si no hay error, entonces devuelve el      **/
/**       primer parametro; en caso contrario muestra el error  **/
/**       producido.                                            **/
/** Tipo de resultado: real de doble precision "double"         **/
/** Parametro:                                                  **/
/**  Nombre: d                                                  **/
/**    	Tipo: real de doble precision "double"                  **/
/**     Mision: almacenar el valor obtenido al aplicar una      **/
/**            funcion matematica                               **/
/**  Nombre: s                                                  **/
/**    	Tipo: cadena de caracteres o puntero a caracter "char *"**/
/**     Mision: indica el nombre de la funcion que se ha        **/
/**       utilizado para calcular el valor del primer parametro.**/
/*****************************************************************/

double errcheck(double d, char *s)
{
 if (errno==EDOM)
    {
     errno=0;
     execerror(s," argumento fuera de dominio ");
    }
   else if (errno==ERANGE)
           {
            errno=0;
            execerror(s," resultado fuera de rango ");
           }
 return d;
}
