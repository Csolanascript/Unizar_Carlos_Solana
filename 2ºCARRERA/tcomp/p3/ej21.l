%{
#include "y.tab.h"
#include <math.h>
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
[0-9]+	{yylval = atoi(yytext); return(NUMBER);}
[0-9]+"b"	{yylval = atoi(yytext);
		int result = 0;
		int i = 0;
		while (yylval != 0) {
			result += yylval%10*pow(base, i);
			yylval = yylval / 10;
			i++;
		}
		yylval = result;
		return(NUMBER);}
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%

/* yylval almacena solo valores tipo int */
/* algoritmo de cambio de base */
/* comando incluyendo -lm para detectar librería necesaria */