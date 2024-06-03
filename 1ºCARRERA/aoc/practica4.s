 	AREA codigo, CODE 
	EXPORT resolverReinas
	;sbr que resuelve el problema de las n reinas
	;r0=num de reinas(por valor)
	;r1= vector con las posiciones de las reinas(por referencia)
resolverReinas 
	PUSH {r11,lr}
	mov  fp,sp
	PUSH {r2}
	
	mov  r2,#0
	
	PUSH {r2}         ;apilamos r2
	PUSH {r1}         ;apilamos r1(solucion)	
	PUSH {r0}         ;apilamos r0 (n)
	bl   colocarReina ;colocarReina(0,solucion,n) 
	add  sp,sp,#12    ;desapilamos los parametros
	 
	POP {r2,r11,pc} 

 
 
colocarReina 
	  PUSH {r11,lr}
	  mov  fp,sp	  ;guardamos stack pointer
	  PUSH {r0-r4}
	
	  LDR  r0,[fp,#8]        ;r0=n
	  LDR  r1,[fp,#12]       ;r1=@solucion
	  LDR  r2,[fp,#16]       ;r2=fila 
	  mov  r3,#0
	  mov  r4,#0
	
	  cmp  r2,r0
	  bhs  exit1             ;en caso de que el numero de filas sea mayor o igual al de reinas entonces entramos al bucle
	  
buc1  str  r3,[r1,r2,LSL #2] ;solucion[fila]=0, es decir, r3=@r1+r2*4

	  sub  sp,sp,#4
	  PUSH {r2}
	  PUSH {r1}
	  bl   comprobarReina    ;comprobarReina(fila,solucion)
	  POP  {r1}
	  POP  {r2}
	  POP  {r4}              ;r4 guardara el true (1) o false (0) que ha devuelto comprobarReina
	  
	  cmp  r4,#1
	  bne  exit2
	  add  r2,r2,#1
	  PUSH {r2}
	  PUSH {r1}
	  PUSH {r0}
	  bl   colocarReina      ;colocarReina(fila+1,solucion,n)
	  POP  {r0}
	  POP  {r1}
	  POP  {r2}
	  sub  r2,r2,#1
	  
exit2 add r3,r3,#1
	  cmp r3,r0
	  blo buc1
	  b   adios
	
	
exit1 PUSH {r1}              ;apilamos @solucion
	  PUSH {r0} 			 ;apilamos n
	  bl   mostrarTablero    ;mostrarTablero(solucion,n)
	  add  sp,sp,#8

adios POP {r0-r4,r11,pc}



comprobarReina
	  PUSH {r11,lr}
	  mov  fp,sp
	  PUSH {r0-r6}
	
	  LDR  r2,[fp,#12]       ;r2=fila 
	  LDR  r1,[fp,#8]        ;r1=@solucion
	  mov  r0,#0 ;r0=i
	  
	  cmp  r2,#0
	  beq  rT

buc2  ldr  r3,[r1,r0,LSL #2] ;solucion[i], es decir, r3=@r1+r0*4
	  ldr  r4,[r1,r2,LSL #2] ;solucion[fila], es decir, r4=@r1+r2*4
	  
	  cmp  r3,r4
	  beq  rF
	  
	  subs r5,r2,r0 
	  bmi  inv1              ;si sale negativo hacemos resta inversa 
vuel  subs r6,r4,r3
	  bmi  inv2
	  b    comp			   
inv1  rsb  r5,r2,r0
	  b    vuel	   
inv2  rsb  r6,r4,r3
comp  cmp  r5,r6
	  bne  exit3
	  
rF    mov  r3,#0
	  str  r3,[fp,#16] 		 ;el resultado de la subrutina sera false
	  b    exit6

exit3 add  r0,r0,#1
	  cmp  r0,r2
	  blo  buc2
	 
rT    mov  r3,#1
	  str  r3,[fp,#16] 		 ;el resultado de la subrutina sera true
     
exit6 POP {r0-r6,r11,pc}

 
 
mostrarTablero
	  PUSH {r11,lr}
	  mov  fp,sp
	  PUSH {r0-r7}
	
	  LDR  r0,[fp,#12]       ;r0=n
	  LDR  r1,[fp,#8]        ;r1=@solucion 
	  mov  r2,#0             ;r2=i=0
	  mov  r5,#'#'           ;r5=#
	  mov  r6,#'-'           ;r6=-
	  LDR  r7,=0x40007E00    ;r7 apunta a la primera posicion del tablero
	
buc3  mov  r3,#0             ;r3=j=0
buc4  ldr  r4,[r0,r2,LSL #2]
      cmp  r4,r3
	  bne  exit4
	  strb r5,[r7,r3]        ;colocamos en el tablero una reina
	  b    exit5

exit4 strb r6,[r7,r3]		 ;colocamos en el tablero un espacio

exit5 add  r3,r3,#1
	  cmp  r3,r1
	  blo  buc4
	  
	  add  r7,r7,#0x00000020 ;pasamos a la siguiente linea de memoria 
	  
	  add  r2,r2,#1
	  cmp  r2,r1
	  blo  buc3
	
	  POP {r0-r7,r11,pc}
			 SS
	  END 