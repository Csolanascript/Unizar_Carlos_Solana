%{
#include <stdio.h>
int base = 10;			
%}

%token NUMBER EOL CP OP
%start calclist
%token ADD SUB 
%token MUL DIV
%token BASE IGUAL

%%
calclist : /* nada */
	| calclist exp EOL { printf("=%d\n", $2); }
	| calclist BASE IGUAL NUMBER EOL { base = $4; }
	;
exp : 	factor 
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }	
	;
factor : 	factor MUL factorsimple { $$ = $1 * $3; }
		| factor DIV factorsimple { $$ = $1 / $3; }
		| factorsimple
		;
factorsimple : 	OP exp CP { $$ = $2; }
		| NUMBER 
		;
%%

int yyerror(char* s) {
	printf("\n%s\n", s);
	return 0;
}
int main() {
  	yyparse();
}

