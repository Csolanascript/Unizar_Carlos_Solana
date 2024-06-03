# Copyright (c) 2012-2015 Juan Segarra, Natalia Ayuso
#
# This file is part of RCFTP daemon.
#
# RCFTP daemon. is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RCFTP daemon is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with RCFTP daemon.  If not, see <http://www.gnu.org/licenses/>.
#
# $Id: Makefile,v 1.3 2014/03/20 13:38:37 jsegarra Exp $

#######################
# Ayuda basica:
#  - las lineas que empiezan con # son comentarios
#  - usar el comando "make"para conseguir el objetivo por defecto (all)
#  - usar el comando "make objetivo" para conseguir "objetivo"
#  - el comando make comprueba las fechas de los ficheros y solo realiza
#      acciones para obtener objetivos NO disponibles o NO actualizados
#######################

#######################
# definicion de variables y constantes
#######################
CC=gcc
UNAME := $(shell uname)
ifeq ($(UNAME), Linux) # equipos del laboratorio L1.02
	RCFTPOPT= -Wall
endif
ifeq ($(UNAME), SunOS) # hendrix
	RCFTPOPT= -Wall -lsocket -lnsl -lrt
endif


#######################
# definicion de objetivos
#######################
# objetivo: ficheronecesario-1 ficheronecesario-2 ... ficheronecesario-n
#		comando-1 necesario para cumplir el objetivo
#		comando-2 necesario para cumplir el objetivo
#		comando-n necesario para cumplir el objetivo
#######################

# objetivo por defecto: tener el fichero rcftpd actualizado
all: rcftpd

# objetivo para obtener rcftpd: compilar los ficheros -o actualizados
rcftpd: rcftpd.o rcftp.o multialarm.o
	$(CC) $(RCFTPOPT) -o rcftpd rcftpd.o rcftp.o multialarm.o

# objetivo para obtener rcftpd.o: compilar los ficheros del servidor rcftpd.c/.h
rcftpd.o: rcftpd.c rcftpd.h
	$(CC) $(RCFTPOPT) -c rcftpd.c

# objetivo para obtener rcftp.o: compilar los ficheros del protocolo rcftp.c/.h
rcftp.o: rcftp.c rcftp.h
	$(CC) $(RCFTPOPT) -c rcftp.c

# objetivo para obtener rcftp.o: compilar los ficheros del protocolo rcftp.c/.h
multialarm.o: multialarm.c multialarm.h
	$(CC) $(RCFTPOPT) -c multialarm.c

# objetivo para comprimir los fuentes
rcftpd.tar.gz: *.c *.h ?akefile
	tar chf rcftpd.tar COPYING *.c *.h ?akefile
	gzip rcftpd.tar

# objetivo para generar documentación
documentacion: html/index.html
html/index.html: *.c *.h
	doxygen -g
	doxygen

# objetivo para limpiar: borra todo lo compilado y no tiene ficheros necesarios
clean:
	-rm -f rcftpd rcftpd.o rcftp.o multialarm.o rcftpd.tar.gz 
	
