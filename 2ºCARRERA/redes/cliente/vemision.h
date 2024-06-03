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


/*********************************************************/
/* Definiciones, cabeceras, etc. para MULTIALARM         */
/*********************************************************/

/**
 * Tamaño de memoria a reservar para la ventana de emisión, en bytes
 */
#define MAXVEMISION 10240

/**************************************************************************/
/* cabeceras de funciones públicas VEMISION                             */
/**************************************************************************/

/**
 * Establece el tamaño de la ventana de emisión (<=MAXVEMISION)
 * @param[in] tamaño a usar
 */
void setwindowsize(unsigned int total);

/**
 * Calcula el espacio libre en la ventana de emisión
 * @return espacio libre en la ventana de emisión
 */
int getfreespace();

/**
 * Añade datos de longitud len a la ventana de emisión
 * @param[in] datos a añadir
 * @param[in] longitud de datos a añadir
 * @return longitud de datos añadidos (=longitud de datos a añadir)
 */
int addsentdatatowindow(char * data, int len);

/**
 * Libera espacio en la ventana de emisión
 * @param[in] número de secuencia (no incluido) hasta el que liberar
 */
void freewindow(uint32_t next);

/**
 * Pide datos para reenviar
 * @param[out] datos a reenviar
 * @param[in/out] longitud de datos solicitados y longitud de datos añadidos
 * @return número de secuencia a poner en los datos
 */
uint32_t getdatatoresend(char * buffer, int *len);

/**
 * Imprime la ventana de emisión
 */
void printvemision();


