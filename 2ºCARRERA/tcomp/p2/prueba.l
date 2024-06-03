%{
#include<math .h>
//comentario ;;;;
int a = 0;
%}

%%    //otro comentario
t+ 	{printf('' hola ;''); a++;}
ggg    {}
''aabb''[A-Z]+ 	{ a++; if (a ==0) {a ++;}
	//otro comentario;;;
return a;}
%%
int main() { // y otro comentario
	yylex();
}
