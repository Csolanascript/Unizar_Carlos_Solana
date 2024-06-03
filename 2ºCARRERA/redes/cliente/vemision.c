/**
 * @file vemision.c vemision.h
 * @brief Funciones para manejar una ventana de emisión en la asignatura "Redes de Computadores", de la U. Zaragoza
 *
 * @author Juan Segarra
 * $Revision$
 * $Date$
 * $Id$
 *
 * Copyright (c) 2013-2016 Juan Segarra, Natalia Ayuso
 *
 * This file is part of VEmision.
 *
 * VEmision is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VEmision is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Multialarm.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <stdint.h>

#include "vemision.h"

/**
 * Cola circular, con datos válidos entre [firstelem,lastelem-1], y datos a REenviar en resendelem
 */
static char vemision[MAXVEMISION];
static unsigned int totalelems=0; // tamaño de ventana a usar
static unsigned int firstelem=0; // primer byte añadido
static unsigned int lastelem=0; // último byte añadido
static unsigned int resendelem=0;
static char vvacia=1; // para diferenciar entre vacía y llena si firstelem==lastelem

/*
 * Número de secuencia del elemento en firstelem
 */
static uint32_t numseqfirst=0;


void setwindowsize(unsigned int total) {
	if (totalelems!=0) {
               fprintf(stderr,"Warning: el tamaño de la ventana ya había sido establecida anteriormente. Ignorando la nueva especificación\n");
	} else if (total>MAXVEMISION) {
               fprintf(stderr,"ERROR: el tamaño especificado para la ventana supera el máximo permitido\n");
	       exit(3);
	} else {
		totalelems=total;
	}
}


int getfreespace() {
	if ((lastelem==firstelem)&&(vvacia))
		return totalelems;
	else
		return (totalelems-(lastelem-firstelem))%totalelems;
}


int addsentdatatowindow(char * data, int len) {
	if (getfreespace()<len) { // no cabe
		//return 0;
		fprintf(stderr,"addsentdatatowindow: intentando añadir más datos (%d B) que el libre (%d B) de la ventana de emisión\n",len,getfreespace());
		exit(3);
	} else { // cabe
		if ((totalelems-lastelem)>len) { // cabe en bloque
			memcpy(&vemision[lastelem],data,len);
		} else { // cabe en dos trozos
			memcpy(&vemision[lastelem],data,totalelems-lastelem);
			memcpy(&vemision[0],&data[totalelems-lastelem],len-(totalelems-lastelem));
		}
		lastelem=(lastelem+len)%totalelems;
		vvacia=0;
		return len;
	}
}

// libera hasta next (no incluido)
void freewindow(uint32_t next) {
	if (((next<=numseqfirst) && (numseqfirst<(uint32_t)(numseqfirst+totalelems-getfreespace()))) // next menor
			|| (next>(uint32_t)(numseqfirst+totalelems-getfreespace()))) { // next mayor
		fprintf(stderr,"freewindow: intentando liberar datos (hasta el número de secuencia %d) no almacenados en la ventana de emisión [%d,%d]\n",next-1,numseqfirst,numseqfirst+totalelems-getfreespace());
		exit(3);
	} else { // ok
		firstelem=(firstelem+(next-numseqfirst))%totalelems;
		numseqfirst=next;
		if (firstelem==lastelem) {
			vvacia=1;
			resendelem=firstelem;
		} else if (((firstelem<=lastelem) && (resendelem<firstelem || resendelem>=lastelem)) ||
		    ((firstelem>lastelem) && (resendelem<firstelem && resendelem>=lastelem))) // arrastra resendelem
			resendelem=firstelem;
		return;
	}
}


uint32_t getdatatoresend(char * buffer, int * len) {
	uint32_t numseq;

	// calculamos si tenemos los len bytes para dar o no
	if (resendelem<lastelem) { // los datos a enviar están ordenados
		if ((lastelem-resendelem)<*len)
			*len=lastelem-resendelem;
	} else { // datos en final e inicio de ventana circular
		if ((totalelems-resendelem+lastelem)<*len)
			*len=totalelems-resendelem+lastelem;
	}

	// calculamos el número de secuencia
	numseq=numseqfirst+(totalelems+resendelem-firstelem)%totalelems;
	// copiamos los datos
	if (resendelem+(*len)<=totalelems) { // todos los datos en bloque
		memcpy(buffer,&vemision[resendelem],*len);
	} else { // datos al final e inicio de ventana
		memcpy(buffer,&vemision[resendelem],totalelems-resendelem);
		memcpy(&buffer[totalelems-resendelem],&vemision[0],*len-(totalelems-resendelem));
	}
	// actualizamos indice
	resendelem=(resendelem+(*len))%totalelems;
	if (resendelem==lastelem)
		resendelem=firstelem;

	return numseq;
}

void printvemision() {
	if ((firstelem==lastelem)&&vvacia)
		printf("[]");
	else if (resendelem==firstelem)
		printf("[(%d)...%d]",numseqfirst,numseqfirst+(totalelems+lastelem-1-firstelem)%totalelems);
	else
		printf("[%d...(%d)...%d]",numseqfirst,numseqfirst+(totalelems+resendelem-firstelem)%totalelems,numseqfirst+(totalelems+lastelem-1-firstelem)%totalelems);
	printf(" \t%d bytes libres\n",getfreespace());
}
