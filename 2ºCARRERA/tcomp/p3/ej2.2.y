%{
#include <stdio.h>
#include <math.h>
#include <errno.h>
int base = 10;			
%}

%token NUMBER EOL BASE IGUAL CP OP ENDOP ENDBOP
%start calclist
%token ADD SUB 
%token MUL DIV 

%%
calclist : /* nada */
	| calclist exp EOL { printf("=%d\n", $2); }
	| calclist BASE IGUAL NUMBER EOL { base = $4; }
	| calclist exp ENDBOP EOL {
		int num = $2;
		int result = 0;
		int i = 1;
		while (num != 0) {
			result += num % base * i;
			num = num / base;
			i = i * 10;
		}
		printf("=%d\n", result);}
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
