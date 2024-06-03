%{
#include<string.h>
#include <math.h>
#include "y.tab.h"
%}

%%
"->"       {return(FLECHA);}
"("        {return(OP);}
")"        {return(CP);}
","        {return(COMA);}
";"        {return(PUNTOCOMA);}
[0-9]+  {yylval.nombre = strdup(yytext); return(NUM);}
\n        {return(EOL);}
[ \t]     {/* ignorar espacios */}				
%%

