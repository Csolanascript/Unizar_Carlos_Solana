%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define PALOS 3
#define DIM 27
#define BUFF 4000
//carlos solana carlos tristan
//declarar la variable listaTr de tipo ListaTransiciones
//Almacena las transiciones de un solo nodo (nodoOrig) 
//a varios nodos (nodosFin y sus correspondientes etiquetas)
struct ListaTransiciones {
	char* nodoOrig;
	char* nodosFin[DIM];
	char* etiquetas[DIM]; 
	int total;
} listaTr;

//tabla de adyacencia
char* tablaTr[DIM][DIM];

//inicializa una tabla cuadrada DIM x DIM con la cadena vacia
void iniTabla(char* tabla[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			tabla[i][j] = "";
		}
	}
}

/*
 * Calcula la multiplicacion simbolica de matrices 
 * cuadradas DIM x DIM: res = t1*t2
 *
 * CUIDADO: res DEBE SER UNA TABLA DISTINTA A t1 y t2
 * Por ejemplo, NO SE DEBE USAR en la forma:
 *           multiplicar(pot, t, pot); //mal
 */
void multiplicar(char* t1[DIM][DIM], char* t2[DIM][DIM], char* res[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			res[i][j] = (char*) calloc(BUFF, sizeof(char));
			for (int k = 0; k < DIM; k++) {
				if (strcmp(t1[i][k],"")!=0 && strcmp(t2[k][j],"") != 0) {
					strcat(strcat(res[i][j],t1[i][k]),"-");
					strcat(res[i][j],t2[k][j]);
				}
			}
		}
	}
}


/* 
 *Copia la tabla orig en la tabla copia
*/
void copiar(char* orig[DIM][DIM], char* copia[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			copia[i][j] = strdup(orig[i][j]);
		}
	}
}

int pasarADecimal (int numero){
	int numDecimal = 0;
	int exponente = 0;
	while (numero != 0) {
		numDecimal = numDecimal + (numero % 10) * pow(PALOS, exponente);
		numero = numero / 10;
		exponente++;
	}
	return numDecimal;
}

%}

  //nuevo tipo de dato para yylval
%union{
	char* nombre;
}

%token FLECHA OP CP COMA PUNTOCOMA NUM EOL
%start inicio	

%type<nombre> NUM
%%
inicio : /* nada */
		| inicio estOrigen FLECHA transiciones PUNTOCOMA EOL	{ listaTr.total = 0; }
		| inicio estOrigen FLECHA transiciones PUNTOCOMA 
;

estOrigen : NUM		{ listaTr.nodoOrig = $1; }
;

transicion : NUM OP NUM CP		{ listaTr.nodosFin[listaTr.total] = $1;
					  listaTr.etiquetas[listaTr.total] = $3; 
					  int origenes = pasarADecimal(atoi(listaTr.nodoOrig)); 
					  int destinos = pasarADecimal(atoi(listaTr.nodosFin[listaTr.total]));
					  tablaTr[origenes][destinos] = listaTr.etiquetas[listaTr.total];
					  listaTr.total++;
					}
;

transiciones : transicion
		| transicion COMA transiciones	
;
%%

int yyerror(char* s) {
	printf("%s\n");
	return -1;
}

int main() {
	listaTr.total = 0;
	iniTabla(tablaTr);

	char* estadoIni = "000";
	char* estadoFin = "222";
	
	int error = yyparse();
	
	if (error == 0) {
		char* pot[DIM][DIM];
		copiar(tablaTr,pot);
		char* tablaAux[DIM][DIM];
		copiar(tablaTr, tablaAux);
		//calcular movimientos de estadoIni a estadoFin
		int estIni = pasarADecimal(atoi(estadoIni));
		int estFin = pasarADecimal(atoi(estadoFin));
		//calculando las potencias sucesivas de tablaTr
		while(strcmp(pot[estIni][estFin], "") == 0){
			multiplicar(tablaTr, tablaAux, pot);
			copiar(pot, tablaAux);
		}

		printf("Nodo inicial  : %s\n", estadoIni);
		printf("Movimientos   : %s\n", pot[estIni][estFin]);
		printf("Nodo final    : %s\n", estadoFin);
	}
	return error;
}
