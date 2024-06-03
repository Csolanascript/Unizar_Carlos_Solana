#######################
# Makefile del programa cliente
#######################

#######################
# Ayuda basica:
#  - las lineas que empiezan con # son comentarios
#  - usar el comando "make" para conseguir el objetivo por defecto (all)
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

# objetivo por defecto: tener el fichero rcftpclient actualizado
all: rcftpclient

# objetivo para obtener rcftpclient: compilar los ficheros objeto -o 
rcftpclient: rcftpclient.o rcftp.o multialarm.o misfunciones.o vemision.o
	$(CC) $(RCFTPOPT) -o rcftpclient rcftpclient.o rcftp.o multialarm.o misfunciones.o vemision.o

# objetivo para obtener rcftpclient.o: compilar los ficheros del cliente rcftpclient.c/.h
rcftpclient.o: rcftpclient.c rcftpclient.h
	$(CC) $(RCFTPOPT) -c rcftpclient.c

# objetivo para obtener rcftp.o: compilar los ficheros del protocolo rcftp.c/.h
rcftp.o: rcftp.c rcftp.h
	$(CC) $(RCFTPOPT) -c rcftp.c

# objetivo para obtener multialarm.o: compilar los ficheros de gestión de timeouts multialarm.c/.h
multialarm.o: multialarm.c multialarm.h
	$(CC) $(RCFTPOPT) -c multialarm.c

# objetivo para obtener vemision.o: compilar los ficheros de gestión de ventana de emsion vemision.c/.h
vemision.o: vemision.c vemision.h
	$(CC) $(RCFTPOPT) -c vemision.c

# objetivo para obtener misfunciones.o: compilar los ficheros propios misfunciones.c/.h
misfunciones.o: misfunciones.c misfunciones.h
	$(CC) $(RCFTPOPT) -c misfunciones.c

# objetivo para comprimir los fuentes tal y como se piden para la entrega
tarea:
	tar chf "tareaRC_${LOGNAME}.tar" *.c *.h ?akefile
	gzip "tareaRC_${LOGNAME}.tar"

# objetivo para limpiar: borra todo lo generado
clean:
	-rm -f rcftpclient rcftpclient.o rcftp.o multialarm.o misfunciones.o vemision.o "tareaRC_${LOGNAME}.tar.gz" 
	
