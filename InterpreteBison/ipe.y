/*

 Ejemplo 9

  Se ha ampliado el ejemplo 8 para permitir
  - operadores relacionales y lógicos
  - sentencia condicional: if
  - sentencia iterativa: while
  - ejecutar las sentencias contenidas en un fichero indicado en la l’nea de comandos
    Por ejemplo:
       > ./ejemplo9.exe factorial.txt
*/

%{
#include <stdio.h>
#include <math.h>

#include "ipe.h"

#include "macros.h"

#define code2(c1,c2)         code(c1); code(c2)
#define code3(c1,c2,c3)      code(c1); code(c2); code(c3)
#define code5(c1,c2,c3,c4,c5)   code(c1); code(c2); code(c3); code(c4); code(c5);
%}

%union{
       Symbol *sym;    /* puntero a la tabla de simbolos */
       Inst *inst;     /* instruccion de maquina */
}

%token FIN_SENTENCIA
%token <sym> NUMBER VAR CADENA CONSTANTE FUNCION0_PREDEFINIDA FUNCION1_PREDEFINIDA FUNCION2_PREDEFINIDA INDEFINIDA READ READ_CHAR PRINT PRINT_CHAR IF THEN ELSE END_IF WHILE 
DO END_WHILE REPEAT UNTIL FOR SINCE INC END_FOR TOKEN_LUGAR TOKEN_BORRAR
%type <inst> stmt asgn expr  stmtlist cond mientras si fin para variable repetir /*exprcadena*/
%right ASIGNACION
%left O_LOGICO
%left Y_LOGICO
%left MAYOR_QUE MENOR_QUE MENOR_IGUAL MAYOR_IGUAL IGUAL DISTINTO_QUE
%left CONCATENACION
%left '+' '-'
%left '*' '/' MODULO POTENCIA
%left UNARIO NEGACION
%right '^'
%%

list :    /* nada: epsilon produccion */
        | list stmt  FIN_SENTENCIA {code(STOP); return 1;}
        | list error FIN_SENTENCIA   {yyerrok;}
        ;

stmt :    /* nada> epsilon produccion */  {$$=progp;}
        | asgn          {code(pop2);}
	      | PRINT '(' expr ')'    {code(escribir); $$ = $3;}
        | READ '(' VAR ')'    {code2(leervariable,(Inst)$3);}
        | READ_CHAR '(' VAR ')' { code2(leer_cadena,(Inst)$3);}
        | PRINT_CHAR '(' expr ')' { code(escribir_cadena); $$ = $3;}
        | TOKEN_LUGAR '(' expr ',' expr ')'     {code(colocarCursor);    $$=$3;}        
        | TOKEN_BORRAR    {code(borrarPantalla); $$=progp;}
        //Bucle while
        | mientras cond DO stmtlist END_WHILE fin
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del bucle */
                   ($1)[2]=(Inst)$6; /* siguiente instruccion al bucle */
                  }

        //if simple
        | si cond THEN stmtlist END_IF fin /* proposicion if sin parte else */
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del if */
                   ($1)[3]=(Inst)$6; /* siguiente instruccion al if */
                  }
        //if con else
        | si cond THEN stmtlist fin ELSE stmtlist END_IF fin /* proposicion if con parte else */
                  {
                   ($1)[1]=(Inst)$4; /* cuerpo del if */
                   ($1)[2]=(Inst)$7; /* cuerpo del else */
                   ($1)[3]=(Inst)$9; /* siguiente instruccion al if-else */
                  }
        //bucle para
        | para variable SINCE expr fin UNTIL expr fin INC expr fin DO stmtlist END_FOR fin
        {
          ($1)[1] = (Inst)$7; //condicion de parada
          ($1)[2] = (Inst)$10; //incremento
          ($1)[3] = (Inst)$13; //cuerpo del bucle
          ($1)[4] = (Inst)$15; //siguiente instruccion
        }
        /*| '{' stmtlist '}'  {$$ = $2;} */

        //Repetir hasta
         | repetir stmtlist fin UNTIL cond fin
                  {
                  ($1)[1]=(Inst)$5; /* condicion del bucle */
                  ($1)[2]=(Inst)$6; /* siguiente instruccion al bucle */
                  }
        ;


asgn :    VAR ASIGNACION expr { $$=$3; code3(varpush,(Inst)$1,assign);}
       /* | VAR_CADENA ASIGNACION expr { $$=$3; code3(varpush,(Inst)$1,assign);}*/
        | CONSTANTE ASIGNACION expr
          {execerror(" NO se pueden asignar datos a constantes ",$1->nombre);}
	;

variable:  VAR {code((Inst)$1); $$=progp;}
        ;

cond :    '(' expr ')' {code(STOP); $$ =$2;}
        ;

mientras:    WHILE      {$$= code3(whilecode,STOP,STOP);} //uno es para el cuerpo y el otro para el siguiente
        ;

repetir:    REPEAT      {$$= code3(dowhilecode,STOP,STOP);}
        ;

si:       IF         {$$= code(ifcode); code3(STOP,STOP,STOP);}
        ;

fin :    /* nada: produccion epsilon */  {code(STOP); $$ = progp;}
        ;

para:   FOR {$$=code5(forcode,STOP,STOP,STOP,STOP);}
        ;

stmtlist:  /* nada: prodcuccion epsilon */ {$$=progp;}
        | stmtlist stmt FIN_SENTENCIA
        ;

expr :    NUMBER     		{$$=code2(constpush,(Inst)$1);}
        | VAR        		{$$=code3(varpush,(Inst)$1,eval);}
        | CONSTANTE      	{$$=code3(varpush,(Inst)$1,eval);}
        | asgn
        | FUNCION0_PREDEFINIDA '(' ')'      {code2(funcion0,(Inst)$1->u.ptr);}
        | FUNCION1_PREDEFINIDA '(' expr ')' {$$=$3;code2(funcion1,(Inst)$1->u.ptr);}
        | FUNCION2_PREDEFINIDA '(' expr ',' expr ')'
                                            {$$=$3;code2(funcion2,(Inst)$1->u.ptr);}
        | '(' expr ')'  	{$$ = $2;}
        | expr '+' expr 	{code(sumar);}
        | expr '-' expr 	{code(restar);}
        | expr '*' expr 	{code(multiplicar);}
        | expr '/' expr 	{code(dividir);}
        | expr MODULO expr 	{code(modulo);}
        | expr POTENCIA expr 	{code(potencia);}
        |'-' expr %prec UNARIO 	{$$=$2; code(negativo);}
        |'+' expr %prec UNARIO 	{$$=$2; code(positivo);}
        | expr MAYOR_QUE expr 	{code(mayor_que);}
        | expr MAYOR_IGUAL expr {code(mayor_igual);}
        | expr MENOR_QUE expr 	{code(menor_que);}
        | expr MENOR_IGUAL expr {code(menor_igual);}
        | expr IGUAL expr 	{code(igual);}
        | expr DISTINTO_QUE expr 	{code(distinto);}
        | expr Y_LOGICO expr 	{code(y_logico);}
        | expr O_LOGICO expr 	{code(o_logico);}
        | NEGACION expr 	{$$=$2; code(negacion);}
        | CADENA        {$$=code2(charpush,(Inst)$1);}
        | expr CONCATENACION expr {code(concatenar); printf("chivato\n");}
      	;

/*exprcadena :    CADENA        {$$=code2(charpush,(Inst)$1);}
        | VAR_CADENA {$$=code3(varpush,(Inst)$1,eval_cadena);}
        | exprcadena CONCATENACION exprcadena {code(concatenar);}
        ;*/


%%

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf begin;
char *progname;
int lineno = 1;
/* Dispositivo de entrada est‡ndar de yylex() */
extern FILE * yyin;

main(int argc, char *argv[])
{

 void fpecatch();

 /* Si se invoca el intérprete con un fichero de entrada */
 /* entonces se establece como dispositivo de entrada para yylex() */
 if (argc == 2) yyin = fopen(argv[1],"r");


 progname=argv[0];

 /* inicializacion de la tabla de simbolos */
 init();

/* Establece un estado viable para continuar despues de un error */
 setjmp(begin);

 /* Establece cual va a ser la funcion para tratar errores de punto flotante */
 signal(SIGFPE,fpecatch); /* Excepcion de punto flotante*/

/* initcode inicializa el vector de intrucciones y la pila del interprete */
 for (initcode(); yyparse(); initcode()) execute(prog);

 return 0;

}

yyerror(char *s)
{
 warning(s,(char *) 0);
}

warning(char *s, char *t)
{
 fprintf(stderr," ** %s : %s", progname,s);
 if (t) fprintf(stderr," ---> %s ",t);
 fprintf(stderr,"  (linea %d)\n",lineno);
}

execerror(s,t) /* recuperacion de errores durante la ejecucion */
char *s,*t;
{
 warning(s,t);
  longjmp(begin,0);
}

void fpecatch()     /*  atrapa errores de punto flotante */
{
 execerror("error de punto flotante ",(char *)0);
}

