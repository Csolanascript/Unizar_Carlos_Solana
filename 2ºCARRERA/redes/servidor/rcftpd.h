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
/* Definiciones, cabeceras, etc. del servidor (daemon) rcftpd     */
/*                                                                */
/* En hendrix, compilar con: cc -lsocket -lnsl -o rcftpd rcftpd.c */
/******************************************************************/

/* Valores por defecto de red a simular, en microsegundos */
/** @{ */
#define T_PROP 250000 /**< Tiempo de propagación, en microsegundos */
#define T_TRANS 200000 /**< Tiempo de transmisión, en microsegundos */
#define ERR_FREQ 5 /**< Inversa de la tasa de error */
/** @} */

/* máximo número de mensajes pendientes de enviar */
/* la ventana de emisión del cliente ha de ser menor */
#define WINDOWSIZE 1024 /**< Número máximo de mensajes pendientes de enviar */

/* Flags del programa */
/** @{ */
#define F_VERBOSE	0x1	/**< Flag para mostrar detalles en la salida estándar */
// a bailar!
#define F_SALSA		0x2	/**< Flag para generar respuestas incorrectas y descartando mensajes recibidos */
#define F_FUNKY		0x4	/**< F_SALSA + puede no responder o duplicar mensaje */
#define F_ROCKNROLL	0x8 /**< F_FUNKY + cualquier error, con/sin descartar mensajes recibidos */
/** @} */

/* defines para la salida del programa */
/** @{ */
#define S_OK 0 /**< Flag de salida correcta */
#define S_ABORT 1 /**< Flag de salida incorrecta */
#define S_SYSERROR 2 /**< Flag de salida por error de sistema */
#define S_PROGERROR 3 /**< Flag de salida por error de programa (BUG!) */
#define S_CLIERROR 4 /**< Flag de salida para avisar de error en cliente */
/** @} */

/* defines de tipos de error provocado */
/** @{ */
/* -a0-3: sin error (-a0-3) */
#define E_NONE -1 /**< todo correcto */
/* -a1-3: con respuesta incorrecta y descartando mensaje recibido */
#define E_VERSION_LOST 0 /**< versión incorrecta y recepción incorrecta */
#define E_NEXT_MUCHLOWER_LOST 1 /**< next incorrecto y recepción incorrecta */
#define E_SUM_LOST 2 /**< checksum incorrecto y recepción incorrecta */
#define E_SUMSWAP_LOST 3 /**< checksum incorrecto (bytes desordenados) y recepción incorrecta */
// -a2-3: sin respuesta y descartando mensaje recibido, o mensaje duplicado
#define E_KILL_LOST 4 /**< error en la red (mensaje del cliente perdido) */
#define E_EXTRA 5 /**< error en la red (mensaje duplicado) */ 
// -a3: con respuesta incorrecta o sin respuesta, y recepción correcta
#define E_NEXT_LOWER 6 /**< next menor, pero correcto */
#define E_VERSION 7 /**< versión incorrecta */
#define E_NEXT_MUCHLOWER 8 /**< next incorrecto */
#define E_SUM 9 /**< checksum incorrecto */
#define E_SUMSWAP 10 /**< checksum incorrecto (bytes desordenados) */
#define E_KILL 11 /**< error en la red (mensaje del servidor perdido) */
// maximos
#define E_SALSA_MAX 3 /**< Máximo id de errores SALSA */
#define E_FUNKY_MAX 5 /**< Máximo id de errores SALSA+FUNKY */
#define E_ROCKNROLL_MAX 11 /**< Máximo id de errores SALSA+FUNKY+ROCKNROLL */
/** @} */

/* defines para colorear salida */
/** @{ */
#define ANSI_COLOR_RED     "\x1b[31m" /**< Pone terminal a rojo */
#define ANSI_COLOR_GREEN   "\x1b[32m" /**< Pone terminal a verde */
#define ANSI_COLOR_YELLOW  "\x1b[33m" /**< Pone terminal a amarillo */
#define ANSI_COLOR_BLUE    "\x1b[34m" /**< Pone terminal a azul */
#define ANSI_COLOR_MAGENTA "\x1b[35m" /**< Pone terminal a magenta */
#define ANSI_COLOR_CYAN    "\x1b[36m" /**< Pone terminal a turquesa */
#define ANSI_COLOR_RESET   "\x1b[0m" /**< Desactiva color del terminal */
/** @} */


/**
 * Lee argumentos en línea de comandos, pone flags y alimenta el generador de números aleatorios
 *
 * @param[in] argc Número de argumentos
 * @param[in] argv Lista de argumentos
 * @param[out] flags Flags del programa a partir de los argumentos
 * @param[out] port String con el servicio/número de puerto
 * @param[out] ttrans Tiempo de transmisión a simular, en microsegundos
 * @param[out] tprop Tiempo de propagación a simular, en microsegundos
 * @param[out] error_frequency Inversa de la tasa de errores a generar (si hay que generar errores)
 */
void initargs(int argc, char **argv, unsigned int *flags, char** port, unsigned long *ttrans, unsigned long *tprop, int *error_frequency);

/**
 * Imprime un resumen del uso del programa
 *
 * @param[in] progname Nombre del comando
 */
void printuso(char *progname);


/**
 * Inicializa el servidor en el socket
 *
 * @param[in] port String con el servicio o número de puerto
 * @return El descriptor de socket UDP abierto
 */
int start_server(char* port);

/**
 * Manejo de los mensajes de un cliente hasta confirmar el último
 *
 * @param[in] s	Socket UDP del que esperar mensajes
 * @param[in] flags Flags del programa a tener en cuenta en la ejecución
 * @param[in] ttrans Tiempo de transmisión a simular, en microsegundos
 * @param[in] tprop Tiempo de propagación a simular, en microsegundos
 * @param[in] error_frequency Inversa de la tasa de errores a generar (si hay que generar errores)
 */
void process_requests(int s, unsigned int flags, unsigned long ttrans, unsigned long tprop, int error_frequency);

/**
 * Imprime el otro extremo del socket
 *
 * @param[in] caddr Dirección de socket
 */
void print_peer(struct sockaddr_storage caddr);

/**
 * Genera un identificador de error de forma aleatoria
 *
 * @param[in] flags Flags del programa a tener en cuenta en la ejecución
 * @param[in] error_frequency Inversa de la tasa de errores a generar (si hay que generar errores)
 * @return Tipo de error a producir
 */
int get_random_error(unsigned int flags,int error_frequency);

/**
 * Genera un mensaje erróneo a partir de un mensaje correcto
 *
 * @param[in,out] sendbuffer Mensaje (correcto) a enviar (puede alterarse en la función)
 * @param[in] flags Flags del programa a tener en cuenta en la ejecución
 * @param[in,out] error Tipo de error a introducir en el mensaje (puede generar otro)
 * @param[in] next_valido NEXT valido anterior al actual, por si hay que simular recepción incorrecta
 * @param[in] next_calculado NEXT a enviar si no hubiera error
 * @return Número de veces a enviar el mensaje (0,1,2)
 */
int generar_mensaje_erroneo(struct rcftp_msg *sendbuffer, unsigned int flags, int *error, uint32_t next_valido, uint32_t next_calculado);

/**
 * Calcula el siguiente next expected a partir del número de secuencia y longitud,
 * y escribe en el fichero "f_recibido" lo que corresponda
 * 
 * @param[in] oldexpected Next expected viejo
 * @param[in] numseq Número de secuencia del mensaje (host order)
 * @param[in] len Longitud del buffer del mensaje (host order)
 * @param[in] buffer Buffer de datos recibidos
 * @param[in] fsalida Fichero al que escribir los datos
 * @param[in,out] flags Flags: se añadirá F_ABORT si no se puede escribir en fichero
 * @param[in] prgflags Flags del programa
 * @return Next expected (host order)
 */
uint32_t calcnextexpected(uint32_t oldexpected, uint32_t numseq, uint16_t len, 
		uint8_t* buffer, FILE *fsalida, uint8_t *flags, unsigned int prgflags);

/** Envía un mensaje a la dirección especificada
 *
 * @param[in] s Socket
 * @param[in] sendbuffer Mensaje a enviar
 * @param[in] remote Dirección a la que enviar
 * @param[in] remotelen Longitud de la dirección especificada
 * @param[in] flags Flags del programa
 */
void enviamensaje(int s,struct rcftp_msg sendbuffer, struct sockaddr_storage remote, socklen_t remotelen, unsigned int flags);

/**
 * Determina si un mensaje es válido o no
 *
 * @param[in] recvbuffer Mensaje a comprobar
 * @return 1: es el esperado; 0: no es el esperado
 */
int mensajevalido(struct rcftp_msg recvbuffer); 


/**
 * Envía respuesta a interlocutor distinto (inmediatamente, con F_BUSY, sin errores)
 *
 * @param[in] s Socket
 * @param[in] remote Dirección a la que enviar
 * @param[in] remotelen Longitud de la dirección especificada
 * @param[in] flags Flags del programa
 */
void responderbusy(int s, struct sockaddr_storage remote,socklen_t remotelen,unsigned int flags);

/**
 * Devuelve una cadena de descripción del error
 *
 * @param[in] error Número de error
 * @return Cadena con descripción del error
 */
char* strerrorrcftpd(int error);

/**
 * Recibe un mensaje (y hace las verificaciones de error oportunas) 
 *
 * @param[in] socket Descriptor de socket
 * @param[out] buffer Espacio donde almacenar lo recibido
 * @param[in] buflen Longitud del buffer
 * @param[out] remote Dirección de la que hemos recibido
 * @param[out] remotelen Longitud de la dirección de la que hemos recibido
 * @return Tamaño del mensaje recibido
 */
ssize_t recibirmensaje(int socket, struct rcftp_msg *buffer, int buflen, struct sockaddr_storage *remote, socklen_t *remotelen);
