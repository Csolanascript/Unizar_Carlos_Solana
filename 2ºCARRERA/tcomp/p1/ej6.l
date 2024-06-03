%{
#include <stdio.h>

int contadorTL = 0;
int contadorTB = 0;
int contadorTC = 0;
int contadorTN = 0;
int contadorTS = 0;
int contadorTCons = 0;
%}

%%
^[ ]*\n				        {contadorTB++; contadorTL++;}
\n				            {contadorTL++;}
[0-9]+				        {contadorTN++; contadorTC = contadorTC + yyleng;}
.				              {contadorTC++;}
[.,:;].*$		        	{contadorTS++; contadorTC = contadorTC + yyleng;}
^[ ]*[^aeiouAEIOU]		{contadorTCons++; contadorTC = contadorTC + yyleng;}

%%
int main () {
	int error = yylex ();
	printf (" TL : %d\n" , contadorTL);
	printf (" TB : %d\n" , contadorTB);
	printf (" TC : %d\n" , contadorTC);
	printf (" TN : %d\n" , contadorTN);
	printf (" TS : %d\n" , contadorTS);
	printf (" TCons : %d\n" , contadorTCons);
}