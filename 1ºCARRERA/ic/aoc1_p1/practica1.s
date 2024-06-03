		AREA datos,DATA,READWRITE	; area de datos
serie	DCD 1,2,4,6,8,7,9			; serie es una tabla de 7 enteros
long	EQU .-serie						; long=28 (bytes que ocupa serie)
resul	DCB 0						; resultado

    	AREA prog,CODE,READONLY		; area de codigo
    	ENTRY						; primera instruccion a ejecutar
								
    	eor r0,r0,r0				; r0=0 (r0=direcciones datos)
    	mov r1,r0					; r1=r0=0 (r1=resultado suma) 
   
    	LDR r2,=serie				; r2=direccion comienzo tabla serie
buc 	ldr r3,[r2,r0]				; r3=Mem[r2+r0]
    	add r1,r1,r3				; r1=r1+serie[i] (i=0,...,7)
    	add r0,r0,#4				; r0=r0+4 (r0 apunta siguiente elem)
    	cmp r0,#long				; comparar r0 con long
    	bne buc						; salta a buc si r0<>long
    
    	LDR r2,=resul				; r2=direccion resul
    	strb r1,[r2]				; Mem[r2]=r1 (almacena resultado)
       
fin 	b fin						; fin de programa

    	END							; fin de ensamblado