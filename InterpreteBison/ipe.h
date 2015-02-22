typedef struct Symbol
         { /* entrada en la tabla de simbolos */
             char *nombre;
	     short tipo;  /* NUMBER,VAR,FUNCION,INDEFINIDA,CONSTANTE*/
	     struct {
		    double val;        /* VAR, NUMBER, INDEFINIDA, CONSTANTE */
		    double (*ptr)();   /* FUNCION */
            char * cadena;
         } u;
             struct Symbol * siguiente;
         } Symbol;

Symbol *install(), *lookup();
Symbol *install_cadena();

typedef struct Datum { /* tipo de la pila del interprete */
                     double val;
                     Symbol *sym;
                     char * cadena;
                    } Datum;

extern Datum pop();
extern pop2();

typedef int (*Inst)(); /* instruccion maquina */
#define STOP (Inst) 0

extern Inst prog[], *progp, *code();

extern  assign(), constpush(), dividir(), escribir(), eval(), funcion0(),
        funcion1(),funcion2(), modulo(), multiplicar(), negativo(), positivo(),
        potencia(), restar(), sumar(), varpush(), ifcode(), whilecode(),
        mayor_que(), menor_que(), mayor_igual(),menor_igual(), igual(),
       distinto(), y_logico(), o_logico(), negacion(), leervariable(), concatenar(),
       leer_cadena(), eval_cadena(), escribir_cadena(), charpush(), forcode(), dowhilecode(), borrarPantalla(), colocarCursor();
