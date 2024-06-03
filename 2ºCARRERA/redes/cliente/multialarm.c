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



#include <stdio.h>
#include <stdlib.h> 
#include <signal.h> // sigaction(), sigsuspend(), sig*()
#include <unistd.h> // alarm()
#include <sys/time.h> // gettimeofday()
#include <time.h> // nanosleep()
#include <errno.h>

#include "multialarm.h"

/**
 * Contador de timeouts vencidos.
 * Uso: comparar con otra variable inicializada a 0; si son distintas, tratar un timeout e incrementar en uno la otra variable.
 */
volatile int timeouts_vencidos=0;

/**
 * Duración de los timeouts, en microsegundos
 */
static int duracion_timeout=0;

/**
 * Tiempo de transmisión a simular, parando el proceso, en microsegundos
 */
static struct timespec tiempo_transmision;

/*
 * Cola circular, con elementos válidos entre [firstelem,lastelem-1]
 */
static struct timeval tv[MAXALARMS];
static unsigned int firstelem=0;
static unsigned int lastelem=0;


/**************************************************************************/
/* Gestor de interrupción de alarma */
/**************************************************************************/
void handle_sigalrm(int sig) {
	timeouts_vencidos++;
	signal(SIGALRM,handle_sigalrm);
	canceltimeout();
	/* Lo recomendable sería llamar a setnextalarm() desde el programa principal, al detectar un timeout vencido, pero por legibilidad lo situamos aquí */
}

/**************************************************************************/
/* Especifica la duración del timeout a utilizar y el T_t a simular */
/**************************************************************************/
void settimeoutduration(unsigned long usec, unsigned long usleep) {
	if (duracion_timeout!=0) {
		fprintf(stderr,"Warning: la duración del timeout ya había sido establecida anteriormente. Ignorando la nueva especificación\n");
	} else if (usec==0) {
		fprintf(stderr,"Error: No se puede especificar una duración de 0\n");
		exit(3);
	} else {
		duracion_timeout=usec;
		tiempo_transmision.tv_sec=usleep/1000000;
		tiempo_transmision.tv_nsec=(usleep%1000000)*1000;
	}
}

/**************************************************************************/
/* Añade una alarma para saltar dentro de duracion_timeout microsegundos */
/* Duerme el proceso durante el tiempo de transmisión especificado */
/* Devuelve 1 si se ha podido añadir; 0 si no se ha podido añadir */
/**************************************************************************/
int addtimeout() {
	struct timezone tz;
	struct itimerval timeout;

	if (duracion_timeout==0) {
		fprintf(stderr,"Error: intentando añadir un timeout sin haber especificado su duración\n");
		exit(3);
	}
	if (firstelem==((lastelem+1)%MAXALARMS)) { // vector lleno
		fprintf(stderr,"addtimeout: No se ha podido añadir alarma; se ha alcanzado el límite de alarmas pendientes (%d)\n",MAXALARMS);
		//exit(3);
		return 0;
	} else { // añadimos tiempo actual al vector
		if (gettimeofday(&tv[lastelem],&tz)==-1) {
			perror("Error en gettimeofday");
			exit(2);
		}
		if (firstelem==lastelem) { // ninguna alarma activa
			//fprintf(stderr,"Activando alarma\n");
			timeout.it_value.tv_sec=duracion_timeout/1000000;
			timeout.it_value.tv_usec=duracion_timeout%1000000;
			timeout.it_interval.tv_sec=0;
			timeout.it_interval.tv_usec=0;
			setitimer(ITIMER_REAL,&timeout,NULL);
		}
		//fprintf(stderr,"Añadiendo alarma\n");
		lastelem=(lastelem+1)%MAXALARMS;
		//fprintf(stderr,"Alarmas activas: %d\n",(MAXALARMS+(lastelem-firstelem))%MAXALARMS);

		// dormimos el tiempo requerido para realizar la transmisión
		// si nos interrumpen, no hacemos nada (podríamos ponernos a dormir otra vez)
		if ((nanosleep(&tiempo_transmision,NULL)==-1) && (errno!=EINTR)) {
			perror("Error en nanosleep");
			exit(2);
		}

		return 1;
	}
}


/**************************************************************************/
/* Añade una alarma para saltar en max(duracion_timeout,timeoutanterior+delay) microsegundos */
/* NO HAY QUE ALTERNAR LLAMADAS addtimeout y adddelayedtimeout: podrían desordenar el vector */
/* Devuelve 1 si se ha podido añadir; 0 si no se ha podido añadir */
/**************************************************************************/
int adddelayedtimeout(unsigned long delay) {
	struct timezone tz;
	struct itimerval timeout;
	struct timeval delayed;

	if (duracion_timeout==0) {
		fprintf(stderr,"Error: intentando añadir un timeout sin haber especificado su duración\n");
		exit(3);
	}
	if (firstelem==((lastelem+1)%MAXALARMS)) { // vector lleno
		fprintf(stderr,"addtimeout: No se ha podido añadir alarma; se ha alcanzado el límite de alarmas pendientes (%d)\n",MAXALARMS);
		//exit(3);
		return 0;
	} else { // añadimos tiempo actual al vector
		if (gettimeofday(&tv[lastelem],&tz)==-1) {
			perror("Error en gettimeofday");
			exit(2);
		}
		if (getnumtimeouts()!=0) { // hay un timeout anterior
			delayed.tv_sec=tv[(lastelem+MAXALARMS-1)%MAXALARMS].tv_sec
					+(tv[(lastelem+MAXALARMS-1)%MAXALARMS].tv_usec+delay)/1000000;
			delayed.tv_usec=(tv[(lastelem+MAXALARMS-1)%MAXALARMS].tv_usec+delay)%1000000;
			if (delayed.tv_sec>tv[lastelem].tv_sec || (delayed.tv_sec==tv[lastelem].tv_sec&&
					delayed.tv_usec>tv[lastelem].tv_sec)) { // si el retardado es mayor
				tv[lastelem].tv_sec=delayed.tv_sec;
				tv[lastelem].tv_usec=delayed.tv_usec;
			}
		} else {
		//if (firstelem==lastelem) { // ninguna alarma activa
			//fprintf(stderr,"Activando alarma\n");
			timeout.it_value.tv_sec=duracion_timeout/1000000;
			timeout.it_value.tv_usec=duracion_timeout%1000000;
			timeout.it_interval.tv_sec=0;
			timeout.it_interval.tv_usec=0;
			setitimer(ITIMER_REAL,&timeout,NULL);
		}
		//fprintf(stderr,"Añadiendo alarma\n");
		lastelem=(lastelem+1)%MAXALARMS;
		//fprintf(stderr,"Alarmas activas: %d\n",(MAXALARMS+(lastelem-firstelem))%MAXALARMS);
		return 1;
	}
}



/**************************************************************************/
/* Actualiza el vector de alarmas pendientes, los timeouts vencidos y programa la siguiente alarma, si existe. Debe llamarse desde el programa principal para cancelar un timeout (el más antiguo) que aún no ha vencido. */
/**************************************************************************/
int canceltimeout() {
	long timeelapsed;
	struct timeval tactual;
	struct timezone tz;
	char buscar_alarma=1;
	struct itimerval timeout;

	if (getnumtimeouts()==0) {
		fprintf(stderr,"Error: no queda ningún timeout que cancelar\n");
		return 0;
	}
		
	while (buscar_alarma) {
		//fprintf(stderr,"Quitando alarma\n");
		firstelem=(firstelem+1)%MAXALARMS; // quitamos alarma vencida
		if (firstelem!=lastelem) { // hay alarmas pendientes
			if (gettimeofday(&tactual,&tz)==-1) {
				perror("Error en gettimeofday");
				exit(2);
			}
			timeelapsed=1000000*(tactual.tv_sec-tv[firstelem].tv_sec);
			timeelapsed+=tactual.tv_usec-tv[firstelem].tv_usec;
			if (duracion_timeout-timeelapsed<0)
				timeouts_vencidos++;
			else
				buscar_alarma=0;
		} else // no hay alarmas pendientes
			buscar_alarma=0;
	}
	if (firstelem!=lastelem) {// hay alarmas pendientes
		if (duracion_timeout-timeelapsed<10) // damos margen minimo a alarmas muy seguidas
			timeelapsed=duracion_timeout-10;
		//fprintf(stderr,"Activando alarma en %ld mus\n",duracion_timeout-timeelapsed);
		timeout.it_value.tv_sec=(duracion_timeout-timeelapsed)/1000000;
		timeout.it_value.tv_usec=(duracion_timeout-timeelapsed)%1000000;
		timeout.it_interval.tv_sec=0;
		timeout.it_interval.tv_usec=0;
		setitimer(ITIMER_REAL,&timeout,NULL);
		return 1;
	} else {
		//fprintf(stderr,"No hay alarmas pendientes\n");
		timeout.it_value.tv_sec=0;
		timeout.it_value.tv_usec=0;
		timeout.it_interval.tv_sec=0;
		timeout.it_interval.tv_usec=0;
		setitimer(ITIMER_REAL,&timeout,NULL);
		return 0;
	}
}

/**************************************************************************/
/* Devuelve el número de timeouts programados (pendientes de vencer) */
/**************************************************************************/
int getnumtimeouts() {
	return ((lastelem+MAXALARMS)-firstelem)%MAXALARMS;
}

