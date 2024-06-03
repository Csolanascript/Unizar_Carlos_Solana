/****************************************************************************/
/* Main y funciones ya implementadas del cliente (rcftpclient)                    */
/* Plantilla $Revision$ */
/****************************************************************************/

/**************************************************************************/
/* INCLUDES                                                               */
/**************************************************************************/
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
#include <sys/time.h>
#include <math.h>
#include "rcftp.h"
#include "rcftpclient.h"
#include "multialarm.h"
#include "misfunciones.h"


/**************************************************************************/
// Solaris 5.10 (Hendrix) no dispone de la función timersub en <sys/time.h>
// El siguiente código es un parche para poder compilar en Hendrix
#ifndef timersub
# define timersub(a, b, result) \
       do {                                                            \
               (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;           \
               (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;        \
               if ((result)->tv_usec < 0) {                            \
                       --(result)->tv_sec;                             \
                       (result)->tv_usec += 1000000;                   \
               }                                                       \
       } while (0)
#endif


/**************************************************************************/
/* VARIABLES GLOBALES                                                     */
/**************************************************************************/
// para estadísticas de velocidad efectiva
static unsigned int numbytesleidos=0;

// variable para indicar si mostrar información extra durante la ejecución
// como la mayoría de las funciones necesitaran consultarla, la definimos global
char verb;

// variable externa que muestra el número de timeouts vencidos
// Uso: Comparar con otra variable inicializada a 0; si son distintas, tratar un timeout e incrementar en uno la otra variable
extern volatile const int timeouts_vencidos;

// variable externa (misfunciones.c) con la cadena de autores a mostrar
extern char* autores;


/**************************************************************************/
/* MAIN                                                                   */
/**************************************************************************/
int main(int argc,char *argv[]) {
	struct timeval horainicio; // variable para estadísticas finales
    int	sock; // descriptor del socket a usar
	int alg; // algoritmo a usar
    char *port,*dest; // punteros a strings especificando puerto y destino
	struct addrinfo *servinfo=NULL; // puntero a dirección del servidor
	unsigned int window; // tamaño de la ventana deslizante
	unsigned long ttrans; // tiempo de transmisión a simular
	unsigned long timeout; // tiempo de expiración a simular

	/* imprimir nombre de autores */
	printf("%s\n",autores);

	/* leer parametros de entrada */
    initargs(argc,argv,&verb,&alg,&window,&ttrans,&timeout,&dest,&port);

	/* obtener estructura de direccion del servidor */
	servinfo=obtener_struct_direccion(dest, port, verb);
    /* configurar socket */
    sock=initsocket(servinfo,verb);

	/* inicializamos los tiempos a simular */
	settimeoutduration(timeout,ttrans);
	
	/* anotamos la hora antes de empezar la transmisión */
	if (gettimeofday(&horainicio,NULL)<0) {
		perror("Error al intentar obtener la hora del sistema\n");
		exit(1);
	}

    /* algoritmos para realizar envio */
	switch(alg) {
		case 1: alg_basico(sock,servinfo); break;
		case 2: alg_stopwait(sock,servinfo); break;
		case 3: alg_ventana(sock,servinfo,window); break;
		default: printf("Algoritmo desconocido\n"); break;
	}

	/* muestra info y calcula la velocidad efectiva conseguida (aproximadamente) */
	muestrainforesumen(horainicio);
	exit(0);
}



/**************************************************************************/
/* readtobuffer -- lee de la entrada estándar no más de RCFTP_BUFLEN */
/**************************************************************************/
int readtobuffer(char * buffer, int maxlen) {
	ssize_t len;

	// verificamos que lo que queremos leer es razonable
	if (maxlen<0) {
		fprintf(stderr,"Error: readtobuffer: intentando leer %d bytes de datos\n",maxlen);
		exit(1);
	} else if (maxlen > RCFTP_BUFLEN) {
		fprintf(stderr,"Warning: readtobuffer: intentando leer más de RCFTP_BUFLEN bytes\n");
	} else if ((maxlen<RCFTP_BUFLEN) && verb) {
		fprintf(stderr,"Warning: readtobuffer: intentando leer menos de RCFTP_BUFLEN bytes\n");
	}

	len = read(0, buffer, maxlen);
	// lee del teclado; sobreescribe buffer; devuelve el número de bytes leídos
	//
	// bloqueante: no sale hasta haber leído algo o error
	// no bloqueante: sale si no hay nada que leer
	//
	// en caso de Segmentation fault/Violación de segmento aquí, es que
	// no le estamos pasando correctamente la dirección de un buffer de tamaño adecuado
	
	// verificamos la cantidad de datos leída
	if (len<0 && errno!=EAGAIN) { // no mostramos mensaje en caso de descriptor no bloqueante
		perror("Error: readtobuffer: error al leer de la entrada estándar: ");
		exit(1);
	}
	if(verb) {
		if (len==0) {
			printf("readtobuffer: fin de fichero alcanzado. No quedan datos que leer\n");
		} else if (len==maxlen) {
			printf("readtobuffer: leídos de la entrada estándar los %zd bytes solicitados\n",len);
		} else if (len>0) {
			printf("readtobuffer: leídos %zd bytes de la entrada estándar (fin de fichero alcanzado o teclado en entrada estándar)\n",len);
		}
	}

	if (len>0) { // para el caso normal, anotamos los bytes leídos
		numbytesleidos+=len;
	}

	return len;
}


/**************************************************************************/
/* muestrainforesumen -- Muestra info y calcula el tiempo transcurrido y la velocidad efectiva aproximada */
/**************************************************************************/
//void muestrainforesumen(time_t horainicio) {
void muestrainforesumen(struct timeval horainicio) {
	struct timeval horafin;
	struct timeval intervalo;
	double segundos;
	
	if (gettimeofday(&horafin,NULL)<0) {
		perror("Error al intentar obtener la hora del sistema\n");
		exit(1);
	}

	timersub(&horafin,&horainicio,&intervalo);
	segundos=intervalo.tv_sec+0.000001*intervalo.tv_usec;

	printf("--------------- información de la comunicación ---------------\n");
	printf("Datos válidos de usuario enviados: %d bytes\n",numbytesleidos);
	printf("Tiempo transcurrido: %f segundos\n",segundos);
	printf("Tiempos de expiración vencidos: %d\n",timeouts_vencidos);
	if (numbytesleidos!=0) {
		printf("Velocidad efectiva conseguida: %f bits/segundo\n",numbytesleidos*8/segundos);
	} else {
		printf("Velocidad efectiva conseguida: 0 bits/segundo\n");
	}

	return;
}
	

/**************************************************************************/
/* Imprime un resumen del uso del programa */
/**************************************************************************/
void printuso(char *progname) {
    fprintf(stderr,"Uso: %s [-v] -a[alg] [-t[Ttrans]] [-T[timeout]] [-w[tam]] -d<dirección> -p<puerto>\n",progname);
	fprintf(stderr,"  -v\t\tMuestra detalles en salida estándar\n");
	fprintf(stderr,"  -a[alg]\tAlgoritmo de secuenciación a utilizar:\n");
	fprintf(stderr,"      1\t\tAlgoritmo básico\n");
	fprintf(stderr,"      2\t\tAlgoritmo Stop&Wait\n");
	fprintf(stderr,"      3\t\tAlgoritmo de ventana deslizante Go-Back-n\n");
	fprintf(stderr,"  -t[Ttrans]\tTiempo de transmisión a simular, en microsegundos (por defecto: 200000)\n");
	fprintf(stderr,"  -T[timeout]\tTiempo de expiración a simular, en microsegundos (por defecto: 1000000)\n");
	fprintf(stderr,"  -w[tam]\tTamaño (en bytes) de la ventana de emisión (sólo usado con -a3) (por defecto: 2048)\n");
	fprintf(stderr,"  -d<dirección>\tDirección del servidor\n");
	fprintf(stderr,"  -p<puerto>\tServicio o número de puerto del servidor\n");
}


/**************************************************************************/
/* initargs -- read command line parameters */
/**************************************************************************/
void initargs(int argc, char **argv, char *verb, int* alg, unsigned int* window, unsigned long* ttrans, unsigned long* timeout, char** dest, char** port) {
    char *progname = *argv;

	// default values
	*verb=0;
	*window=2048;
	*ttrans=200000;
	*timeout=1000000;
	// error values
	*alg=0;
	*dest=NULL;
    *port=NULL;

    if (argc<2) {
		fprintf(stderr,"Número de párametros incorrecto\n");
		printuso(progname);
		exit(1);   	
    }
    for(argc--,argv++; argc > 0; argc--,argv++) {
    	if (**argv == '-') {
    		switch (*(++*argv)) {
    		case 'v':
    			*verb=1;
    			break;

    		case 'a':
    			*alg=atoi(++*argv);
    			break;

    		case 'w':
    			*window=atoi(++*argv);
    			break;

    		case 't':
    			*ttrans=strtoul(++*argv,NULL,10);
    			break;

    		case 'T':
    			*timeout=strtoul(++*argv,NULL,10);
    			break;

    		case 'd':
    			*dest=(++*argv);
    			break;

    		case 'p':
    			*port=(++*argv);
    			break;

    		default:
				printuso(progname);
    			exit(1);
    		}
    	}
    	else {
			printuso(progname);
    		exit(1);
    	}
    }

    if (*port==NULL) {
		fprintf(stderr,"Puerto no especificado\n");
		printuso(progname);
		exit(1);    	
    }
	else if (*dest==NULL) {
		fprintf(stderr,"Destino no especificado\n");
		printuso(progname);
		exit(1);    	
    }
	else if (*alg==0) {
		fprintf(stderr,"Algoritmo no especificado correctamente\n");
		printuso(progname);
		exit(1);    	
    }
	else if (*window<=0) {
		fprintf(stderr,"Ventana no especificada correctamente\n");
		printuso(progname);
		exit(1);    	
    }
	else if	(*ttrans==0) {
		fprintf(stderr,"Tiempo de transmisión no especificado correctamente\n");
		printuso(progname);
		exit(1);    	
    }
	else if	(*timeout==0) {
		fprintf(stderr,"Tiempo de expiración no especificado correctamente\n");
		printuso(progname);
		exit(1);    	
    }

	if (*verb) {
		fprintf(stderr,"Valores de parámetros: a=%d, w=%d, tt=%ld, T=%ld, d=%s, p=%s\n",*alg,*window,*ttrans,*timeout,*dest,*port);
	}	
}


/**************************************************************************/
/*  imprime direccion */
/**************************************************************************/
void print_peer(struct sockaddr_storage * caddr) {
	struct sockaddr_in * caddr_ipv4;
	struct sockaddr_in6 * caddr_ipv6;
	void *addr;
	char *ipver, ipstr[INET6_ADDRSTRLEN];
	unsigned short port;
	
	if (caddr->ss_family== AF_INET) {//IPv4
        caddr_ipv4=((struct sockaddr_in *)((struct sockaddr *)caddr));
        addr = &(caddr_ipv4->sin_addr);
        port = ntohs(caddr_ipv4->sin_port); 
        ipver = "IPv4";
        }else if (caddr->ss_family== AF_INET6) {//IPv6
        caddr_ipv6=((struct sockaddr_in6 *)((struct sockaddr *)caddr));
        addr = &(caddr_ipv6->sin6_addr);
        port = ntohs(caddr_ipv6->sin6_port);
        ipver = "IPv6";
        }else{
        fprintf(stderr, "Error: protocolo desconocido");
        exit(1);
    }
    //convierte la ip a una string y la imprime
    inet_ntop(caddr->ss_family, addr, ipstr, sizeof ipstr);
    printf("Comunicación con el equipo %s usando %s a través del puerto %d\n", ipstr,ipver,port);
    return;
}

