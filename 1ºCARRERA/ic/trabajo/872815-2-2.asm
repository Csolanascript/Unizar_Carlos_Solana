.data 75
cadena:		.ascii16 "CARLOS SOLANA."		; 0043 0041 0052 004C 004F 0053 0020 0053 004F 004C 0041 004E 0041 002E

cero:			.dw 0						;int cero = 0
punto:		.ascii16 '.'				;char16_t punto = 002E
letra:		.rw 1						;char letra
const:		.dw 128					;inst const = 128
espacio:		.ascii16 ' '				;char16_t espacio = 0020

tty_out:		.rw 1						;cout para escribir en pantalla

.code		
0			mov cero, tty_out				;borrar_pantalla()
1			mov cadena, letra				;cadena = cadena[0]
2			mov letra, tty_out			;cout<<letra
3WHILE 		cmp letra, punto				;while(letra != punto){
4			beq HALT					;
5			add const, INST				;i = i+ 1
6INST 		mov cadena, letra				;letra = cadena [i]
7			mov letra, tty_out			;cout<<letra
8			cmp cero, cero				;}
9			beq WHILE					;
HALT 			cmp cero,cero				;if( 0 == 0){
	
			beq HALT					;}









