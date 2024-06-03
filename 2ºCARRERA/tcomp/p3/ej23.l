%{
#include "y.tab.h"
#include <errno.h>
%}

%%
"+"	{return(ADD);}
"-"	{return(SUB);}
"*"	{return(MUL);}
"/"	{return(DIV);}
"("	{return(OP);}
")"	{return(CP);}
":="	{return(VALOR);}
"acum"	{return(ACUM);}
[0-9]+	{yylval = atoi(yytext); return(NUMBER);}
\n	{return(EOL);}
[ \t]	{/* ignorar espacios */}
.	{return(yytext[0]); /* caracter inesperado */}
%%

/* yylval almacena solo valores tipo int */
/* comando incluyendo -lm para detectar librería necesaria */