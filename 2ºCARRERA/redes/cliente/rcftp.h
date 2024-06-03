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
 
/*********************************************************/
/* Definiciones, cabeceras, etc. para el protocolo RCFTP */
/*********************************************************/

#include <netinet/in.h>

/**
 * Longitud de buffer de datos
 */
#define RCFTP_BUFLEN 512

/**
 * Versión del protocolo
 */
#define RCFTP_VERSION_1 1

/**
 * Flag por defecto
 */
#define F_NOFLAGS	0
/**
 * Flag de ocupado atendiendo a otro cliente
 */
#define F_BUSY		1
/**
 * Flag de intención/confirmación de finalizar transmisión
 */
#define F_FIN   	2
/**
 * Flag de aviso de finalización forzosa
 */
#define F_ABORT 	4

/**
 * Estructura para el formato de mensaje RCFTP
 *
 * Aunque la estructura está organizada de forma que el compilador no necesite
 * alinear nada, es recomendable indicárselo, y la forma de hacerlo depende del compilador.
 * Contiene atributo específico de gcc para especificar un struct "empaquetado",
 * es decir, con todos sus campos contiguos en memoria
 */ 
#ifdef __GNUC__
struct __attribute__ ((packed)) rcftp_msg {
#else
struct rcftp_msg {
#endif
    uint8_t	version;		/**< Versión RCFTP_VERSION_1; cualquier otro es inválido */
    uint8_t	flags;			/**< Flags. Máscara de bits de los defines F_X */
    uint16_t	sum;		/**< Checksum calculado con xsum */
    uint32_t	numseq;		/**< Número de secuencia, medido en bytes */
    uint32_t	next;		/**< Siguiente numseq esperado, medido en bytes */
    uint16_t	len;		/**< Longitud de datos válidos, no cabeceras */
    uint8_t	buffer[RCFTP_BUFLEN];	/**< Datos, de longitud fija RCFTP_BUFLEN (512)*/
};



/**************************************************************************/
/* cabeceras de funciones RCFTP                                           */
/**************************************************************************/

/**
 * Imprime los campos de un mensaje RCFTP
 * @param[in]	mensaje	Mensaje a imprimir
 * @param[in]	len	longitud del mensaje
 */
void print_rcftp_msg(struct rcftp_msg *mensaje, int len);


/**
 * Imprime el significado de los flags con texto
 * @param[in] flags Flags a interpretar
 */
void print_flags(uint8_t flags);


/**
 * Comprueba el checksum de un mensaje
 * 
 * @param[in] mensaje Mensaje a comprobar
 * @param[in] len Longitud a verificar 
 * @return 1: mensaje válido; 0: mensaje inválido
 */
int issumvalid(struct rcftp_msg *mensaje,int len);


/**
 * Calcula una suma de 16-bit con acarreo.
 *   Nice feature of sum with carry is that it is byte order independent
 *   carry from hi-bit of one byte goes into lo-bit of other byte.
 *   NOTE: assumes data in buf is in net format.
 *   NOTA2: No hay que aplicar htons al valor recibido.
 *
 * @param[in] buf Datos sobre los que calcular el checksum
 * @param[in] len Longitud de los datos (en bytes)
 * @return Campo Checksum de RCFTP (suma negada)
 */
uint16_t xsum(char *buf, int len);

