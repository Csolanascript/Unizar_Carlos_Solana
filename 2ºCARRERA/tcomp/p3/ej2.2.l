%{
#include "y.tab.h"
#include <errno.h>
extern int base;
%}

%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}
"="	{return(IGUAL);}
"b"	{return(BASE);}  
";b"	{return(ENDBOP);}
";"	{return(ENDOP);}
[0-9]+	{yylval = atoi(yytext); return(NUMBER);}
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%

/* yylval almacena solo valores tipo int */
/* comando incluyendo -lm para detectar librería necesaria */
/* algoritmo de cambio de base no necesario a la hora de detectar los caracteres */