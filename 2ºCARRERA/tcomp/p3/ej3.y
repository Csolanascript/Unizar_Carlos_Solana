%{
#include <stdio.h>
#include <errno.h>
%}

%token X Y Z EOL
%start inicio

%%
inicio : /* vacío */
	| inicio c X s EOL
	| EOL
	;

s : /* vacío */
	| c X s
	;

b : X c Y
	| X c
	;

c : X b X
	| Z
	;
%%

int yyerror(char* s){
	printf("\n%s\n", s);
	return 0;
}
int main(){
	yyparse();
}