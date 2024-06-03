%{
#include "y.tab.h"
#include <errno.h>
%}

%%
"x"	{return(X);}
"y"	{return(Y);}
"z"	{return(Z);}
\n	{return(EOL);}
%%

/* Reconoce los caracteres de la secuencia */