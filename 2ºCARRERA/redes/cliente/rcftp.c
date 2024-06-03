/**
 * @file rcftp.c rcftp.h 
 * @brief Protocolo de comunicaciones RCFTP para la asignatura "Redes de Computadores", de la U. Zaragoza
 * 
 * @author Juan Segarra
 * $Revision$
 * $Date$
 * $Id$
 * 
 * Copyright (c) 2012-2015 Juan Segarra, Natalia Ayuso
 * 
 * This file is part of RCFTP daemon.
 *
 * RCFTP daemon. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RCFTP daemon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RCFTP daemon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <netinet/in.h>
#include <stdio.h>
#include <stdint.h>
#include "rcftp.h"


int issumvalid(struct rcftp_msg *mensaje,int len) {
	if (xsum((char*)mensaje,len)==0)
		return 1;
	else
		return 0;
}


void print_flags(uint8_t flags) {
	char hayflags=0;

	if (flags==0)
		printf("sin flags");
	else if (flags>=(2*F_ABORT))
		printf("valor de flags no válido");
	else {
		if ((flags/F_BUSY)%2==1) {
			printf("ocupado");
			hayflags=1;
		}
		if ((flags/F_FIN)%2==1) {
			if (hayflags) printf(", ");
			printf("fin");
			hayflags=1;
		}
		if ((flags/F_ABORT)%2==1) {
			if (hayflags) printf(", ");
			printf("abortar");
			hayflags=1;
		}
	}
}


void print_rcftp_msg(struct rcftp_msg *mensaje, int len) {
	uint16_t aux;

	if (len!=sizeof(struct rcftp_msg)) {
		printf("Error: el tamaño del mensaje recibido (%d) no es el esperado (%zd)\n",len,sizeof(struct rcftp_msg));
		printf("Imposible interpretar mensaje\n");
	} else {

	printf("\tVersión: %u\n",mensaje->version);
	printf("\tFlags: %u (",mensaje->flags);
	print_flags(mensaje->flags);
	printf(")\n");
	printf("\tNúm. secuencia: %u\n",ntohl(mensaje->numseq));
	printf("\tNext: %u\n",ntohl(mensaje->next));
	printf("\tLen: %u\n",ntohs(mensaje->len));
	printf("\tChecksum (net): 0x%x",ntohs(mensaje->sum));
	/* aplicamos ntohs en el printf solo para que se muestre lo mismo en cualquier equipo
	 * pero NUNCA HAY QUE APLICAR TRANSFORMACIONES SOBRE EL CHECKSUM */
	if (issumvalid(mensaje,len))
		printf(" (ok)\n");
	else {
		printf(" (error, esperaba ");
		aux=mensaje->sum;
		mensaje->sum=0;
		printf("0x%x)\n",ntohs(xsum((char*)mensaje,sizeof(struct rcftp_msg))));
		mensaje->sum=aux;
	}
	}
}


uint16_t xsum(char *buf, int len) {
  register uint16_t	*sp;
  register int16_t	slen;
  register uint32_t	sum;		/* >= 32-bit space to keep sum */
  union { uint16_t s; uint8_t c[2]; } xun;
  int unaligned;

  sum = 0;

  unaligned = ((intptr_t)buf) & 0x1;
  /* If buffer isn't short aligned, need to get fisst byte */
  if (unaligned != 0) {
    xun.s = 0;
    xun.c[1] = buf[0];
    sum = xun.s;
    buf++; len--;
  }
  slen = len/2;			/* length in shorts */

  /* LINT NOTE: next line has possible ptr alignment message, even
     though we've made sure that buf is aligned */
  for(sp = (uint16_t *)buf; slen > 0; slen--,sp++) {
    sum += *sp;
  }

  /* is there a trailing odd byte? */
  if ((len & 0x1) != 0) {
#ifdef DEBUG
      printf("xsum extra: 0x%X (sum: 0x%X, len %d.)\n", buf[len-1], sum, len);
#endif
    xun.s = 0; xun.c[0] = buf[len - 1];
    sum += xun.s;
  }

  /* Fold in all the carries to get a single 16 bit value */
  sum = (sum & 0xFFFF) + (((uint32_t)(sum & 0xFFFF0000))>>16);
  if (sum > 0xFFFF)
    { sum = (sum & 0xFFFF) + 1; }

  if (unaligned != 0)          /* byteswap */
    { sum = ((sum & 0xFF)<<8) + ((sum & 0xFF00)>>8); }
  //return (sum);
  return (~sum);
}
