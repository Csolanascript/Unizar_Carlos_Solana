/**
 * @file rcftpd.c rcftpd.h
 * @brief Servidor con protocolo RCFTP para la asignatura "Redes de Computadores", de la U. Zaragoza
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

/******************************************************************/
/* Implementación de funciones del servidor (daemon) rcftpd       */
/*                                                                */
/* En hendrix, compilar con: gmake                                */
/******************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "rcftp.h"
#include "rcftpd.h"
#include <signal.h>
#include "multialarm.h"

// el servidor utiliza multialarm y timeouts para simular el retardo de la red
// por eso el valor de TIMEOUT debe ser menor al del cliente
extern volatile const int timeouts_vencidos;


/**************************************************************************/
/* MAIN                                                                   */
/**************************************************************************/

/**
 * Programa principal: procesa argumentos, activa el socket y procesa mensajes
 */
int main(int argc,char *argv[]) {
	int	s;
	unsigned int	prgflags;
	char *port;
	unsigned long ttrans=T_TRANS,tprop=T_PROP; // timeout_cliente < 2 ttrans + 2 tprop
	// Estadísticamente, uno de cada "error_frequency" mensajes debería ser erróneo.
	int error_frequency = ERR_FREQ;

	printf("rcftpd $Revision$\n");
	initargs(argc,argv,&prgflags,&port,&ttrans,&tprop,&error_frequency);

	/* start server up */
	if ((s = start_server(port)) < 0) { 
		exit(S_SYSERROR); 
	}

	/* process requests */
	process_requests(s,prgflags,ttrans,tprop,error_frequency);

	close(s);
	printf("Recepción finalizada. Compara los ficheros para verificar los datos recibidos.\n");
	exit(S_OK);
}


/**************************************************************************/
/* Imprime un resumen del uso del programa */
/**************************************************************************/
void printuso(char *progname) {
	fprintf(stderr,"Uso: %s -p<puerto> [-v] [-a[alg]] [-e[frec]] [-t[Ttrans]] [-r[Tprop]]\n",progname);
	fprintf(stderr,"  -p<puerto>\tEspecifica el servicio o número de puerto\n");
	fprintf(stderr,"  -v\t\tMuestra detalles en salida estándar\n");
	fprintf(stderr,"  -a[alg]\tAjusta el comportamiento al algoritmo del cliente (por defecto: 0):\n");
	fprintf(stderr,"      0:\tSin mensajes incorrectos\n");
	fprintf(stderr,"      1:\tFuerza mensajes incorrectos hasta su corrección\n");
	fprintf(stderr,"      2:\tFuerza mensajes incorrectos/pérdidas/duplicados hasta su corrección\n");
	fprintf(stderr,"      3:\tFuerza mensajes incorrectos/pérdidas/duplicados\n");
	fprintf(stderr,"  -e[frec]\tFuerza en media un mensaje incorrecto de cada [frec] (por defecto: %d)\n",ERR_FREQ);
	fprintf(stderr,"  -t[Ttrans]\tTiempo de transmisión a simular, en microsegundos (por defecto: %d)\n",T_TRANS);
	fprintf(stderr,"  -r[Tprop]\tTiempo de propagación a simular, en microsegundos (por defecto: %d)\n",T_PROP);
	fprintf(stderr,"Nota: Los algoritmos 1-3 generan errores aleatoriamente. Además, los algoritmos 1 y 2\nmantienen el error generado hasta que el cliente responda correctamente.\n");
}

/**************************************************************************/
/* initargs - read flags, set flags bits and seed random number generator */
/**************************************************************************/
void initargs(int argc, char **argv, unsigned int *flags, char** port, unsigned long *ttrans, unsigned long*tprop, int *error_frequency) {
	char *progname = *argv;
	int algcli=0;

	*flags = 0;
	*port=NULL;

	if (argc<2) {
		printuso(progname);
		exit(S_ABORT);   	
	}
	for(argc--,argv++; argc > 0; argc--,argv++) {
		if (**argv == '-') {
			switch (*(++*argv)) {
				case 'v':
					*flags |= F_VERBOSE;
					break;

				case 'p':
					*port=(++*argv);
					break;

				case 'a': // algoritmo del cliente
					algcli = atoi(++*argv);
					break;

				case 'e': 
					*error_frequency = atoi(++*argv);
					break;

				case 't':
					*ttrans=strtoul(++*argv,NULL,10);
					break;

				case 'r':
					*tprop=strtoul(++*argv,NULL,10);
					break;

				default:
					printuso(progname);
					exit(S_ABORT);
			}
		}
		else {
			//fprintf(stderr,usage,progname);
			printuso(progname);
			exit(S_ABORT);
		}
	}

	// verifica que haya puerto
	if (*port==NULL) {
		fprintf(stderr,"Puerto no especificado\n");
		printuso(progname);
		exit(S_ABORT);    	
	}

	// configura flags dependiendo del algoritmo del cliente
	if (algcli==0) {  // -a no especificado o especificado 0: sin errores
		;
	} else if (algcli==1) { // algoritmo básico con errores básicos que fuerzan reenvío
		*flags |= F_SALSA;
	} else if (algcli==2) { // stop-wait con errores que fuerzan reenvío
		*flags |= F_FUNKY;
	} else if (algcli==3) { // sliding-window con errores, no fuerzan reenvío
		*flags |= F_ROCKNROLL;
	} else {
		fprintf(stderr,"Algoritmo del cliente desconocido\n");
		printuso(progname);
		exit(S_ABORT);    	
	}

	// ajusta parámetros por defecto si se han especificado mal
	if (*error_frequency == 0) {
		fprintf(stderr,"Asumiendo -e%d\n",ERR_FREQ);
		*error_frequency = ERR_FREQ;
	}
	if (*ttrans==0) {
		fprintf(stderr,"Asumiendo -t%d\n",T_TRANS);
		*ttrans=T_TRANS;
	}
	if (*tprop==0) {
		fprintf(stderr,"Asumiendo -p%d\n",T_PROP);
		*tprop=T_PROP;
	}

	// inicializa aleatoriedad
	srand((int) time((time_t *)0));
}

/**************************************************************************/
/*       start up the server: get a socket */
/**************************************************************************/
int start_server(char* port) {
	int sock, status;
	struct addrinfo hints, *servinfo; 

	// rellenamos los valores de la estructura hints
	memset(&hints, 0, sizeof hints);    //nos aseguramos que la estructura está vacia
	hints.ai_family=AF_UNSPEC;          //sin especificar (AF_UNSPEC)
	hints.ai_socktype=SOCK_DGRAM;      //UDP datagram socket
	hints.ai_flags=AI_PASSIVE;          //mi IP

	// obtiene la información del servidor y la almacena en servinfo
	if ((status=getaddrinfo(NULL,port, &hints, &servinfo))!=0){
		fprintf(stderr,"Error en la llamada getaddrinfo: %s\n",gai_strerror(status));
		exit(S_SYSERROR);
	}

	// crea un extremo de la comunicación y devuelve un descriptor
	if ((sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
		perror("Error en la llamada socket: no se pudo crear el socket");
		exit(S_SYSERROR);
	}

	// asocia socket a puerto
	if (bind(sock, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
		perror("Error asociando el socket");
		exit(S_SYSERROR);
	}

	freeaddrinfo(servinfo);
	printf("Servidor RCFTP en puerto %s\n",port);

	return sock;
}

/**************************************************************************/
/* handle all requests -- does not return unless fatal error              */
/**************************************************************************/
void process_requests(int s, unsigned int progflags, unsigned long ttrans, unsigned long tprop,int error_frequency) {
	ssize_t recvsize;
	struct sockaddr_storage	remote,peer;
	struct rcftp_msg	recvbuffer;
	struct rcftp_msg	sendbuffer,sendbuffer_win[WINDOWSIZE];
	int error,error_win[WINDOWSIZE];
	// firstmsg y lastmsg gestionan y numeran los envíos pendientes
	unsigned int firstmsg=0,lastmsg=0;
	socklen_t remotelen,peerlen;
	FILE * fsalida;
	uint32_t next_calculado, // next calculado a partir del válido
			 next_valido; // next válido (correcto en el servidor)
	int cont,vecesaenviar;
	int sockflags;
	char primeraconexion=1;
	int timeouts_procesados=0;
	int ultimomensajeenviado=0;

	// abrir fichero
	fsalida=fopen("f_recibido","w");
	if (fsalida==NULL) {
		perror("Error al abrir el fichero \"f_recibido\" para escritura");
		exit(S_SYSERROR);
	}
	if (progflags & F_VERBOSE)
		fprintf(stderr,"Fichero \"f_recibido\" abierto para escritura\n");

	// pasamos a socket no bloqueante
	sockflags=fcntl(s,F_GETFL,0);
	fcntl(s,F_SETFL,sockflags|O_NONBLOCK);

	// especificamos el manejador de alarmas
	signal(SIGALRM,handle_sigalrm);

	// establecemos el retardo general a simular en los mensajes: Ttranst+2Tprop
	// se asume que el cliente ya simula un Ttrans
	settimeoutduration(ttrans+2*tprop,0);
	
	// bucle: recibir, procesar mensaje y responder
	while (!ultimomensajeenviado) {

		// recibir mensaje ****************************************************
		recvsize=recibirmensaje(s,&recvbuffer,sizeof(recvbuffer),&remote,&remotelen);

		if (recvsize>0) { // recepción correcta de mensaje

			// verificar si es el primer mensaje ******************************
			if (primeraconexion) {
				primeraconexion=0;
				// a partir de ahora solo atenderemos a peer
				peer=remote;
				peerlen=remotelen;
				if (progflags & F_VERBOSE) {
					print_peer(peer);
				}
				// asumimos que el numseq inicial es el esperado
				next_valido=ntohl(recvbuffer.numseq);
			}

			// print request if in verbose mode *******************************
			if (progflags & F_VERBOSE) {
				printf("\n");
				printf("Mensaje RCFTP " ANSI_COLOR_CYAN "recibido" ANSI_COLOR_RESET ":\n");
				print_rcftp_msg(&recvbuffer,recvsize);
			}

			// si lo recibido no tiene el tamaño esperado, abortar
			if (recvsize!=sizeof(struct rcftp_msg)) {
				fprintf(stderr,"Mensaje con tamaño incorrecto recibido\n");
				exit(S_CLIERROR);
			}

			// si el interlocutor es distinto: responder inmediatamente F_BUSY sin errores
			if ((peerlen!=remotelen) || (memcmp(&remote,&peer,remotelen)!=0)) {
				responderbusy(s,remote,remotelen,progflags);
			} else {
				// mensaje de interlocutor correcto *******************************

				// if flag abort present, abort
				if (recvbuffer.flags & F_ABORT) {
					fprintf(stderr,"Flag F_ABORT recibido\n");
					exit(S_CLIERROR);
				}


				// calcular next ***********************************************
				// empezar sin flags activos
				sendbuffer.flags=F_NOFLAGS;
				// si version,next,checksum ok: escribir datos y calcular nuevo next 
				if (mensajevalido(recvbuffer)) { 
					next_calculado=calcnextexpected(next_valido,ntohl(recvbuffer.numseq), 
							ntohs(recvbuffer.len),recvbuffer.buffer,fsalida,&sendbuffer.flags,progflags);
					// si hemos recibido todo y el interlocutor solicita FIN, contestamos con F_FIN
					if ((next_calculado==(next_valido-(next_valido-ntohl(recvbuffer.numseq))+ntohs(recvbuffer.len))) && (recvbuffer.flags & F_FIN)) {
						sendbuffer.flags|=F_FIN;
					}
				} else { // podríamos ignorar el mensaje, pero mejor dejar claro que es un error
					// el mismo nextexpected
					fprintf(stderr,"Detectado error en cliente\n");
					exit(S_CLIERROR);
				}


				// construir el mensaje válido ***********************************
				// los flags los hemos ido rellenando al calcular el next
				sendbuffer.version=RCFTP_VERSION_1;
				sendbuffer.numseq=htonl(0);
				// sendbuffer.len=htons(adddata()); // nunca respondemos con datos
				sendbuffer.len=htons(0);
				sendbuffer.next=htonl(next_calculado);
				sendbuffer.sum=0;
				sendbuffer.sum=xsum((char*)&sendbuffer,sizeof(sendbuffer));
				// en este punto el mensaje "correcto" está listo


				// generar error **************************************************
				// no forzamos errores con flags activos 
				if (sendbuffer.flags!=F_NOFLAGS) {
					error=E_NONE;
				} else {
					if ((progflags & F_ROCKNROLL) || (error==E_EXTRA) || (next_calculado>next_valido)) {
						error=get_random_error(progflags,error_frequency); // obtener error aleatorio
					} // else (SALSA/FUNKY y no avanzamos), repetir error anterior
				}
				////////////////////////////////////////////////////////////////////////
				// MODIFICAR ESTA VARIABLE PARA FORZAR CIERTO TIPO DE ERROR, por ejemplo:
				// if (error!=E_NONE) error=E_VERSION_LOST;
				////////////////////////////////////////////////////////////////////////


				// construir mensaje erróneo y especificar next_valido ********************
				if (error!=E_NONE) {
					vecesaenviar=generar_mensaje_erroneo(&sendbuffer, progflags, &error, next_valido,next_calculado);
					// descartar datos ya recibidos si el error implica pérdida de datos
					if (error==E_NEXT_LOWER) { // next menor pero correcto
						if (fseek(fsalida,-((long)next_calculado-ntohl(sendbuffer.next)),SEEK_CUR)==-1) {
							perror("Error en fseek");
							exit(S_SYSERROR);
						}
						next_valido=ntohl(sendbuffer.next); // <>next_calculado
					} else if // recepción perdida (*_LOST), que equivale a:
						((error==E_KILL_LOST) || // (envío y recepción perdida, o
						 // distinto de E_NEXT_MUCHLOWER y next<=valido)
						 ((error!=E_NEXT_MUCHLOWER)&&(ntohl(sendbuffer.next)<=next_valido))) {
						if (next_calculado-next_valido>0) { 
							if (fseek(fsalida,-((long)next_calculado-next_valido),SEEK_CUR)==-1) {
								perror("Error en fseek");
								exit(S_SYSERROR);
							}
						}
						//next_valido=next_valido; // <>next_calculado, <>next_enviado
					} else { // next sin error (next_calculado>next_valido)
						next_valido=next_calculado; // =ntohl(sendbuffer->next)
					}
				} else { // E_NONE
					vecesaenviar=1;
					next_valido=next_calculado; // =ntohl(sendbuffer->next)
				}


				// planificamos el envío del mensaje ************************************
				for (cont=0;cont<vecesaenviar;cont++) {
					// control de flujo: no hay que desbordar colas de mensajes ni de alarmas
					if ((firstmsg==(lastmsg+1)%WINDOWSIZE) || (adddelayedtimeout(ttrans)==0)) { 
						fprintf(stderr,"Error en control de flujo: demasiados mensajes recibidos en poco tiempo (el cliente esta desbordando al servidor)\n");
						// deberíamos ignorar el envío, pero mejor enfatizamos que es un error
						exit(S_CLIERROR);
					} else {
						sendbuffer_win[lastmsg]=sendbuffer;
						if (cont==0 && vecesaenviar>1) {
							error_win[lastmsg]=E_NONE;
						} else {
							error_win[lastmsg]=error;
						}
						if (progflags & F_VERBOSE) 
							printf("Planificando envío %d (%s)\n",lastmsg,strerrorrcftpd(error_win[lastmsg]));
						lastmsg=(lastmsg+1)%WINDOWSIZE;
					}
				}
				if (vecesaenviar==0) {
					printf("No planificando envío (%s)\n",strerrorrcftpd(error));
				}
			}
		} // fin de acciones específicas tras una recepción **************************

		// enviamos tantos mensajes como timeouts_vencidos ***************************
		while ((!ultimomensajeenviado) && (timeouts_vencidos>timeouts_procesados)) {
			if (firstmsg==lastmsg) {
				fprintf(stderr,"Error: planificados más envíos que mensajes\n");
				exit(S_PROGERROR);
			}
			if (progflags & F_VERBOSE) {
				printf("\n");
				printf("Realizando envío %d (%s)\n",firstmsg,strerrorrcftpd(error_win[firstmsg]));
			}
			enviamensaje(s,sendbuffer_win[firstmsg],peer,peerlen,progflags);
		
			// realizar acciones dependiendo de flags (solo en envio sin error)
			if ((sendbuffer_win[firstmsg].flags & F_ABORT) && (error_win[firstmsg]==E_NONE)) {
				fprintf(stderr,"Flag F_ABORT transmitido\n");
				exit(S_ABORT);
			}
			if ((sendbuffer_win[firstmsg].flags & F_FIN) && ((error_win[firstmsg]==E_NONE) || (error_win[firstmsg]==E_EXTRA))) {
				if (progflags & F_VERBOSE) {
					printf("Flag F_FIN recibido y confirmado\n");
				}
				ultimomensajeenviado=1;
			}

			firstmsg=(firstmsg+1)%WINDOWSIZE;
			timeouts_procesados++;
		}
	}
}


/**************************************************************************/
/* Recibe mensaje (y hace las verificaciones oportunas) */
/**************************************************************************/
ssize_t recibirmensaje(int socket, struct rcftp_msg *buffer, int buflen, struct sockaddr_storage *remote, socklen_t *remotelen) {
	ssize_t recvsize;
	
	*remotelen = sizeof(*remote);
	recvsize = recvfrom(socket,(char *)buffer,buflen,0,(struct sockaddr *)remote,remotelen);
	if (recvsize<0 && errno!=EAGAIN) { // en caso de socket no bloqueante
		//if (recvsize<0 && errno!=EINTR) { // en caso de socket bloqueante (no funciona en GNU/Linux)
		perror("Error en recvfrom: ");
		exit(S_SYSERROR);
	} else if (*remotelen>sizeof(*remote)) {
		fprintf(stderr,"Error: la dirección del cliente ha sido truncada\n");
		exit(S_SYSERROR);
	}
	return recvsize;
}

/**************************************************************************/
/* Calcula el siguiente next expected y escribe en fichero  */
/**************************************************************************/
uint32_t calcnextexpected(uint32_t oldexpected, uint32_t numseq, uint16_t len, 
		uint8_t* buffer, FILE *fsalida, uint8_t *flags, unsigned int prgflags) {
	size_t wrsize;
	uint16_t firstbyte,bytestowrite;
	uint32_t nextexpected=oldexpected;

	if (len>RCFTP_BUFLEN) {
		fprintf(stderr,"Recibido mensaje informando de longitud %d>%d\n",len,RCFTP_BUFLEN);
		exit(S_CLIERROR);
	}
	if (nextexpected>=numseq &&	nextexpected<numseq+len) {
		if ((nextexpected!=numseq) && (prgflags & F_VERBOSE))
			fprintf(stderr,"Recibido mensaje con numseq=%d cuando esperaba numseq=%d\n(No implica necesariamente que el cliente esté respondiendo mal)\n",numseq,nextexpected);
		firstbyte=nextexpected-numseq;
		bytestowrite=len-firstbyte;
		// guardar datos
		wrsize=fwrite(&buffer[firstbyte],sizeof(char),bytestowrite,fsalida);
		// vaciamos el buffer para poder ver mejor lo escrito
		fflush(fsalida);
		if (wrsize!=bytestowrite) {
			if (wrsize<0) {
				perror("Error al escribir (fwrite) en fichero");
				// el servidor no intenta recuperarse
				*flags|=F_ABORT;
				//nextexpected+=0;
			}
			else {
				fprintf(stderr,"Guardados %d bytes al intentar guardar %d bytes de un mensaje de %d bytes\n",
						(int)wrsize,(int)bytestowrite,(int)len);
				// esto no debería pasar, pero el emisor debería responder bien
				nextexpected+=wrsize;
			}
		} else { // escrito correctamente
			nextexpected+=bytestowrite;
		}
	} else { // números de secuencia fuera de la ventana de recepción
	   	//nextexpected+=0;
		if (prgflags & F_VERBOSE) {
			if (nextexpected!=numseq)
				fprintf(stderr,"Recibido mensaje con numseq=%d cuando esperaba numseq=%d\n(No implica necesariamente que el cliente esté respondiendo mal)\n",numseq,nextexpected);
			else // len==0
				fprintf(stderr,"Recibido un mensaje con longitud de datos igual a %d\n(No implica necesariamente que el cliente esté respondiendo mal)\n",len);
		}
	}
	return nextexpected;
}


/**************************************************************************/
/* Imprime estructura de direccion */
/**************************************************************************/
void print_peer(struct sockaddr_storage caddr) {
	struct sockaddr_in * caddr_ipv4;
	struct sockaddr_in6 * caddr_ipv6;
	void *addr;
	char *ipver, ipstr[INET6_ADDRSTRLEN];
	unsigned short port;

	if (caddr.ss_family== AF_INET) {//IPv4
		caddr_ipv4=((struct sockaddr_in *)((struct sockaddr *)&caddr));
		addr = &(caddr_ipv4->sin_addr);
		port = ntohs(caddr_ipv4->sin_port); 
		ipver = "IPv4";
	}else if (caddr.ss_family== AF_INET6) {//IPv6
		caddr_ipv6=((struct sockaddr_in6 *)((struct sockaddr *)&caddr));
		addr = &(caddr_ipv6->sin6_addr);
		port = ntohs(caddr_ipv6->sin6_port);
		ipver = "IPv6";
	}else{
		fprintf(stderr, "Error: protocolo desconocido");
		exit(S_SYSERROR);
	}
	//convierte la ip a una string y la imprime
	inet_ntop(caddr.ss_family, addr, ipstr, sizeof ipstr);
	printf("Comunicación con el puerto %d del equipo %s usando %s\n",port,ipstr,ipver);
}


/**************************************************************************/
/* obtain the error to introduce                                          */
/**************************************************************************/
int get_random_error(unsigned int flags,int error_frequency) {
	int accion=E_NONE;

	if ((rand()%error_frequency)==0) { 
		if (flags & F_ROCKNROLL) 
			accion=rand() % (E_ROCKNROLL_MAX+1);
		else if (flags & F_FUNKY)
			accion=rand() % (E_FUNKY_MAX+1);
		else if (flags & F_SALSA) 
			accion=rand() % (E_SALSA_MAX+1);
	}

	return accion;
}


/**************************************************************************/
/* generate incorrect response to simulate network trouble                */
/*******************************************************************+******/
int generar_mensaje_erroneo(struct rcftp_msg *sendbuffer, unsigned int flags, int *error, uint32_t next_valido, uint32_t next_calculado) {
	size_t buflen=sizeof(*sendbuffer);
	int enviar=-1;
	union { uint16_t s; char c[2]; } xun;
	char c;

	switch (*error) {
		case E_VERSION_LOST:
			/* asumimos paquete perdido: no actualizamos next */
			sendbuffer->next=htonl(next_valido);
		case E_VERSION:
			/* damage version */
			sendbuffer->version++;
			sendbuffer->sum=0;
			sendbuffer->sum=xsum((char*)sendbuffer,buflen);
			enviar=1;
			break;

		case E_NEXT_LOWER:
			/* next entre 1 y RCFTP_BUFLEN menor */
			if (next_calculado>next_valido) { // solo si hemos recibido datos válidos
				if ((next_calculado-next_valido)>1) // si hemos recibido más de 2 bytes
					sendbuffer->next=htonl(next_calculado-1-(rand()%((next_calculado-next_valido)-1)));
				else // solo hemos recibido 1 byte
					sendbuffer->next=htonl(next_calculado-1);
				sendbuffer->sum=0;
				sendbuffer->sum=xsum((char*)sendbuffer,buflen);
				enviar=1;
				break;
			} // else, generar error E_NEXT_MUCHLOWER_LOST
			fprintf(stderr,"No se ha podido forzar un mensaje con %s",strerrorrcftpd(*error));
			*error=E_NEXT_MUCHLOWER_LOST;
			fprintf(stderr,", intentando uno con %s\n",strerrorrcftpd(*error));
		case E_NEXT_MUCHLOWER_LOST:
		case E_NEXT_MUCHLOWER:
			/* next 2*RCFTP_BUFLEN menor */
			if (next_valido>(2*RCFTP_BUFLEN)) {
					sendbuffer->next=htonl(next_valido-(2*RCFTP_BUFLEN));
					sendbuffer->sum=0;
					sendbuffer->sum=xsum((char*)sendbuffer,buflen);
			} else {
				fprintf(stderr,"No se ha podido forzar un mensaje con %s",strerrorrcftpd(*error));
				*error=E_NONE;
				fprintf(stderr,", intentando uno con %s\n",strerrorrcftpd(*error));
			}
			enviar=1;
			break;

		case E_SUM_LOST:
			/* asumimos paquete perdido: no actualizamos next */
			sendbuffer->next=htonl(next_valido);
		case E_SUM:
			/* damage sum */
			sendbuffer->sum=sendbuffer->sum+1;
			enviar=1;
			break;

		case E_SUMSWAP_LOST:
			/* asumimos paquete perdido: no actualizamos next */
			sendbuffer->next=htonl(next_valido);
		case E_SUMSWAP:
			/* invert checksum */
			xun.s = sendbuffer->sum;
			c = xun.c[0];
			xun.c[0] = xun.c[1];
			xun.c[1] = c;
			sendbuffer->sum = xun.s;
			enviar=1;
			break;

		case E_KILL_LOST:
			/* asumimos paquete perdido: no actualizamos next */
			sendbuffer->next=htonl(next_valido);
		case E_KILL:
			/* don't reply */
			enviar=0;
			break;

		case E_EXTRA:
			/* send extra packet */
			enviar=2;
			break;

		default:
			fprintf(stderr,"Intentando forzar un error desconocido\n");
			exit(S_PROGERROR);
	}
	if (enviar==-1) {
		fprintf(stderr,"Error al generar mensaje erróneo %d\n",*error);
		exit(S_PROGERROR);
	}
	return enviar;
}



/**************************************************************************/
/* Envía un mensaje a la dirección especificada */
/**************************************************************************/
void enviamensaje(int s, struct rcftp_msg sendbuffer, struct sockaddr_storage remote, socklen_t remotelen, unsigned int flags) {
	ssize_t sentsize;

	if ((sentsize=sendto(s,(char *)&sendbuffer,sizeof(sendbuffer),0,(struct sockaddr *)&remote,remotelen)) != sizeof(sendbuffer)) {
		if (sentsize!=-1)
			fprintf(stderr,"Error: enviados %d bytes de un mensaje de %d bytes\n",(int)sentsize,(int)sizeof(sendbuffer));
		else
			perror("Error en sendto");
		exit(S_SYSERROR);
	} 

	// print response if in verbose mode
	if (flags & F_VERBOSE) {
		printf("Mensaje RCFTP " ANSI_COLOR_MAGENTA "enviado" ANSI_COLOR_RESET ":\n");
		print_rcftp_msg(&sendbuffer,sizeof(sendbuffer));
	} 
}	


/**************************************************************************/
/* Verifica version,next,checksum */
/**************************************************************************/
int mensajevalido(struct rcftp_msg recvbuffer) { 
	int esperado=1;
	//uint16_t aux;

	if (recvbuffer.version!=RCFTP_VERSION_1) { // versión incorrecta
		esperado=0;
		fprintf(stderr,"Error: recibido un mensaje con versión incorrecta\n");
	}
	if (recvbuffer.next!=0) { // next incorrecto
		esperado=0;
		fprintf(stderr,"Error: recibido un mensaje con NEXT incorrecto\n");
	}
	if (issumvalid(&recvbuffer,sizeof(recvbuffer))==0) { // checksum incorrecto
		esperado=0;
		fprintf(stderr,"Error: recibido un mensaje con checksum incorrecto\n"); /* (esperaba ");
		aux=recvbuffer.sum;
		recvbuffer.sum=0;
		fprintf(stderr,"0x%x)\n",ntohs(xsum((char*)&recvbuffer,sizeof(recvbuffer))));
		recvbuffer.sum=aux;*/
	}
	return esperado;
}

/**************************************************************************/
/* Responde BUSY a otro interlocutor */
/**************************************************************************/
void responderbusy(int s, struct sockaddr_storage remote,socklen_t remotelen,unsigned int flags) {
	struct rcftp_msg sendbuffer;

	// empezamos a construir el mensaje
	sendbuffer.version=RCFTP_VERSION_1;
	sendbuffer.numseq=htonl(0);
	// longitud=adddata(); // nunca respondemos con datos
	sendbuffer.len=htons(0);
	sendbuffer.next=htonl(0);
	sendbuffer.flags=F_BUSY;  
	sendbuffer.sum=0;
	sendbuffer.sum=xsum((char*)&sendbuffer,sizeof(sendbuffer));

	enviamensaje(s,sendbuffer,remote,remotelen,flags);
}

/**************************************************************************/
/* devuelve una cadena con la descripción del mensaje indicado */
/**************************************************************************/
char* strerrorrcftpd(int error) {
	// intentar coherencia con la frase: "un mensaje con..."
	switch (error) {
		case E_NONE: return ANSI_COLOR_GREEN "todo correcto" ANSI_COLOR_RESET;
		case E_VERSION: return ANSI_COLOR_RED "versión incorrecta" ANSI_COLOR_RESET;
		case E_VERSION_LOST: return ANSI_COLOR_RED "versión incorrecta y recepción incorrecta" ANSI_COLOR_RESET;
		case E_NEXT_LOWER: return ANSI_COLOR_RED "next menor, pero correcto" ANSI_COLOR_RESET;
		case E_NEXT_MUCHLOWER: return ANSI_COLOR_RED "next incorrecto" ANSI_COLOR_RESET;
		case E_NEXT_MUCHLOWER_LOST: return ANSI_COLOR_RED "next incorrecto y recepción incorrecta" ANSI_COLOR_RESET;
		case E_SUM: return ANSI_COLOR_RED "checksum incorrecto" ANSI_COLOR_RESET;
		case E_SUM_LOST: return ANSI_COLOR_RED "checksum incorrecto y recepción incorrecta" ANSI_COLOR_RESET;
		case E_SUMSWAP: return ANSI_COLOR_RED "checksum con bytes desordenados" ANSI_COLOR_RESET;
		case E_SUMSWAP_LOST: return ANSI_COLOR_RED "checksum con bytes desordenados y recepción incorrecta" ANSI_COLOR_RESET;
		case E_KILL: return ANSI_COLOR_RED "error en la red (mensaje del servidor perdido)" ANSI_COLOR_RESET;
		case E_KILL_LOST: return ANSI_COLOR_RED "error en la red (mensaje del cliente perdido)" ANSI_COLOR_RESET;
		case E_EXTRA: return ANSI_COLOR_RED "error en la red (mensaje duplicado)" ANSI_COLOR_RESET;
		default: return ANSI_COLOR_RED "error desconocido" ANSI_COLOR_RESET;
	}
}

