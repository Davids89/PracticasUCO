%{
#include "macros.h"
#include "ipe.h"
#include "ipe.tab.h"
#include "auxiliar.h"
extern char *progname;
extern int lineno;
%}
/* definiciones regulares */
digito          [0-9]
numero          {digito}+(\.{digito}+)?(E[+\-]?{digito}+)?
letra           [a-zA-Z]
subrayado       [_]
identificador   {letra}({letra}|{numero}|{subrayado}({letra}|{numero}))*
cadena          "'"([^']|\\\')*"'"
espacio         [\t\n]
comentario      [\@].*{espacio}*

/* reglas */

%x COMENTARIO

%%
[ \t] { ; } /* saltar los espacios y los tabuladores */

"\{"  {BEGIN COMENTARIO;}
<COMENTARIO>"\}"  {BEGIN 0;}
<COMENTARIO>. {;}

{comentario}    {;}

{cadena}        {
                /* Instala la cadena en la tabla de simbolos */
                //printf("%s", yytext);
                yylval.sym = install_cadena("", CADENA, yytext);

		//printf("Cadena reconocida %s\n",yylval.sym->u.cadena);
                return CADENA;}

{numero}+\.?|{numero}*\.{numero}+ {double d;
                                   sscanf(yytext,"%lf",&d);
                                 /* Instala el numero en la tabla de simbolos */
				   yylval.sym=install("",NUMBER,d);
				   return NUMBER;}

{identificador} { Symbol *s;
                   hacerMinuscula(yytext);
                   if ((s=lookup(yytext)) == 0)
                               s = install (yytext, INDEFINIDA, 0.0);
                    yylval.sym = s;

                    //printf("Lexico: identificador = %s, tipo = %d, cadena = %s\n",yytext,s->tipo, s->u.cadena);
                    return s->tipo == INDEFINIDA ? VAR : s->tipo;}

">="	       {return MAYOR_IGUAL;}
"<="	       {return MENOR_IGUAL;}
"=="	       {return IGUAL;}
">"	           {return MAYOR_QUE;}
"<"	           {return MENOR_QUE;}
"="           {return ASIGNACION;}
"**"           {return POTENCIA;}
"<>"           {return DISTINTO_QUE;}
"||"           {return CONCATENACION;}
";"            {return FIN_SENTENCIA;}


#[Mm][Oo][Dd]   {return MODULO;}
#[Oo]           {return O_LOGICO;}
#[Yy]           {return Y_LOGICO;}
#[Nn][Oo]       {return NEGACION;}




^#       {printf("\n>>>>>>> Fin del programa <<<<<<<\n"); return 0;}
\n      {lineno++; } /* no se devuelve nada, continúa el analisis léxico */
^\$[^\n]*  { /* Ejecucion de un comando del shell */

         BORRAR;
         LUGAR(10,10);
         if (strcmp(yytext,"$") != 0)
            {
             printf("\n Salida provisional al sistema operativo\n\n");
             printf("\n comando ---> %s\n\n",yytext);
             system(yytext+1);
             putchar('\n');
             }
           else
            {
             printf(" Si desea ejecutar un comando del sistema operativo ");
             printf(" teclee: \n\t\t $comando ");
            }

         LUGAR(24,1);
         printf(">>>>>>>> Pulse ENTER para volver al programa %s ",progname);
         /* Espera que se pulse ENTER */
         while(getchar()!='\n')  ;

         BORRAR;
         LUGAR(2,1);
         printf("Continue la ejecucion del programa --> %s \n\n",progname);
         /* no se devuelve nada, continúa el analisis léxico */
         }
.       {return yytext[0];}
