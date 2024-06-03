/* calcOrig.y fichero para la practica 3 de Teoria de la Computacion */
%{
#include <stdio.h>
%}
%token NUMBER ADD SUB MUL DIV EOL CP OP
%start calclist
%%

calclist : /* nada */
	| calclist exp EOL { printf("=%d\n", $2); }
	;
exp : 	factor
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }
	;
factor : 	term
		| exp MUL factor { $$ = $1 * $3; }
		| exp DIV factor { $$ = $1 / $3; }
		;

term : 	NUMBER { printf("number=%d\n", $1); }
	| OP exp CP { $$ = $2; }
	;
%%
int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}
int main() {
  yyparse();
}

