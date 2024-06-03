%{
#include <stdio.h>
%}
%%
^" "+ printf("    ");
^" "*\n
" "+$
" "+ printf(" ");
%%
int main () {
	int error = yylex ();
	return error ;
}
