/**
 * @file multialarm.c multialarm.h
 * @brief Funciones para manejar varias alarmas de forma sencilla en la asignatura "Redes de Computadores", de la U. Zaragoza
 * 
 * @author Juan Segarra
 * $Revision$
 * $Date$
 * $Id$
 * 
 * Copyright (c) 2013-2015 Juan Segarra, Natalia Ayuso
 * 
 * This file is part of Multialarm.
 *
 * Multialarm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Multialarm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Multialarm.  If not, see <http://www.gnu.org/licenses/>.
 */


/*********************************************************/
/* Definiciones, cabeceras, etc. para MULTIALARM         */
/*********************************************************/

#ifndef MULTIALARM // permite múltiples includes sin warnings/errores
#define MULTIALARM

/**
 * Número máximo de alarmas a tener en cuenta
 */
#define MAXALARMS 128

/**************************************************************************/
/* cabeceras de funciones públicas MULTIALARM                             */
/**************************************************************************/

/**
 * Gestor de interrupción de alarma.
 * Necesita asociación a la señal de alarma desde el programa principal.
 */
void handle_sigalrm(int sig);

/**
 * Especifica la duración del timeout a utilizar
 *
 * @param[in] usec Duración del timeout, en microsegundos
 * @param[in] usleep Tiempo de transmisión a simular, parando el proceso, en microsegundos
 */
void settimeoutduration(unsigned long usec, unsigned long usleep);

/**
 *  Añade una alarma para vencer tras duracion_timeout microsegundos
 *  En cada llamada, llama a nanosleep() con el tiempo de transmisión especificado antes
 *
 *  @return 1: timeout añadido; 0: no se ha podido añadir (número máximo alcanzado)
 */
int addtimeout();

/**
 * Actualiza el vector de alarmas pendientes, los timeouts vencidos y programa la siguiente alarma, si existe. Debe llamarse desde el programa principal para cancelar un timeout (el más antiguo) que aún no ha vencido.
 *
 * Además de lo anterior, aunque lo correcto sería usar esta función desde el programa principal cada vez que saltara la alarma, por legibilidad se llama dentro del gestor de interrupción de alarma, con lo que al vencer un timeout no es necesario realizar la llamada.
 *
 * @return 1: timeout cancelado; 0: no hay ningún timeout programado
 */
int canceltimeout();

/**
 * Devuelve el número de timeouts programados (pendientes de vencer)
 *
 * @return Número de timeouts programados (pendientes de vencer)
 */
int getnumtimeouts();

/**************************************************************************/
/* cabeceras de funciones para el SERVIDOR. NO USAR EN EL CLIENTE!! */
/**************************************************************************/

/**
 * Añade una alarma para vencer tras max(duracion_timeout,timeout_anterior+delay) microsegundos.
 * Función para el servidor: NO USAR EN EL CLIENTE.
 * NO HAY QUE ALTERNAR LLAMADAS addtimeout y adddelayedtimeout: podrían desordenar el vector de tiempos.
 * Uso en el servidor: timeout=2*T_t+2*T_p, delay=2*T_t
 * 
 * @param[in] delay Tiempo de transmisión a simular, en microsegundos	
 * @return 1: timeout añadido; 0: no se ha podido añadir (número máximo alcanzado)
 */
int adddelayedtimeout(unsigned long delay);
	
#endif


