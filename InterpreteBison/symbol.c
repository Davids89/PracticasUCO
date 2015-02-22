#include <string.h>
#include <stdlib.h>

#include "ipe.h"
#include "ipe.tab.h" /* Se genera al ejecutar yacc -d hoc5.y */

/* Tabla de simbolos: lista simple encadenada */
static Symbol *symlist=0;

/**********************************************************************/
/** Nombre: lookup                                                   **/
/** Labor que desarrolla: Busca una palabra en la tabla de simbolos. **/
/**        Si la encuentra, devuelve un puntero al nodo que contiene **/
/**        a la palabra; en caso contrario, devuelve NULL.           **/
/** Tipo de resultado: Puntero a la tabla de simbolos "Symbol *"     **/
/** Parametros:                                                      **/
/**  Nombre: s                                                       **/
/**	Tipo:   cadena de caracteres o puntero a char  "char *"      **/
/**     Mision: suministra el nombre de la palabra buscada.          **/
/**********************************************************************/
Symbol *lookup (char *s)
{
	Symbol *sp;

	for (sp=symlist; sp!=(Symbol *) 0 ; sp=sp->siguiente)
		if (strcmp(sp->nombre,s)==0) return sp;
	return 0;
}

/************************************************************************/
/** Nombre: install                                                    **/
/** Labor que desarrolla: Inserta una palabra en la tabla de simbolos, **/
/**        indicando el token que le corresponde y su valor inicial.   **/
/**        Devuelve un puntero al nodo que contiene a la palabra.      **/
/** Tipo de resultado: Puntero a la tabla de simbolos "Symbol *"       **/
/** Parametros:                                                        **/
/**  Nombre: s                                                         **/
/**	Tipo:   cadena de caracteres o puntero a char  "char *"        **/
/**     Mision: suministra el nombre de la palabra a insertar.         **/
/**  Nombre: t                                                         **/
/**	Tipo:   entero "int"                                           **/
/**     Mision: indica el token de la palabra.                         **/
/**  Nombre: d                                                         **/
/**	Tipo:   real de doble precision "double"                       **/
/**     Mision: suministra un valor inicial.                           **/
/************************************************************************/
Symbol *install(char *s, int t, double d)
{
 Symbol *sp;
 char *emalloc();

 sp=(Symbol *) emalloc(sizeof(Symbol));
 sp->nombre=emalloc(strlen(s)+1); /* +1 para el caracter nulo '\0' */
 strcpy(sp->nombre,s);
 sp->tipo=t;
 sp->u.val=d;
 sp->siguiente=symlist;
 symlist=sp;
 return sp;
}

/************************************************************************/
/** Nombre: install_cadena                                             **/
/** Labor que desarrolla: Inserta una palabra en la tabla de simbolos, **/
/**        indicando el token que le corresponde y su valor inicial.   **/
/**        Devuelve un puntero al nodo que contiene a la palabra.      **/
/** Tipo de resultado: Puntero a la tabla de simbolos "Symbol *"       **/
/** Parametros:                                                        **/
/**  Nombre: s                                                         **/
/**	Tipo:   cadena de caracteres o puntero a char  "char *"        **/
/**     Mision: suministra el nombre de la palabra a insertar.         **/
/**  Nombre: t                                                         **/
/**	Tipo:   entero "int"                                           **/
/**     Mision: indica el token de la palabra.                         **/
/**  Nombre: d                                                         **/
/**	Tipo:   cadena de tipo char *				                       **/
/**     Mision: suministra un valor inicial.                           **/
/************************************************************************/

Symbol *install_cadena(char *s, int t, char * d)
{
 Symbol *sp;
 char *emalloc();

 sp=(Symbol *) emalloc(sizeof(Symbol));

 sp->nombre=emalloc(strlen(s)+1); /* +1 para el caracter nulo '\0' */
 strcpy(sp->nombre,s);
 sp->tipo=t;
 sp->u.cadena=(char *)emalloc(strlen(d)+1); /*para guardar la cadena*/
 strcpy(sp->u.cadena,d);

 sp->siguiente=symlist;
 symlist=sp;

 return sp;
}

/************************************************************************/
/** Nombre: emalloc                                                    **/
/** Labor que desarrolla: reserva memoria dinamica y comprueba que no  **/
/**        se producen errores.                                        **/
/**        Devuelve un puntero a la memoria reservada.                 **/
/** Tipo de resultado: Puntero a caracter  "char *"                    **/
/** Parametros:                                                        **/
/**  Nombre: n                                                         **/
/**	Tipo:   entero sin signo "unsigned"                            **/
/**     Mision: indica la cantidad de bytes que se deben reservar      **/
/************************************************************************/

char *emalloc(unsigned n)
{
	char *p;
	p=malloc(n);
	if (p==0) execerror("\nFalta memoria\n",(char *)0);
	return p;
}
