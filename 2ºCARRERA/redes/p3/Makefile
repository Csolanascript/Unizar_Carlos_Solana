# DEFINICION DE VARIABLES Y CONSTANTES
CC=gcc
CFLAGS = -Wall -Wextra -Wshadow
UNAME := $(shell uname)
ifeq ($(UNAME), Linux) # equipos del laboratorio L1.02
	LIBRARIES =
endif
ifeq ($(UNAME), SunOS) # Hendrix
	LIBRARIES = -lsocket -lnsl
endif

# OBJETIVOS
all: clientevocalesUDP servidorvocalesUDP

clientevocalesUDP: clientevocalesUDP.o comun.o
	$(CC) $(CFLAGS) -o clientevocalesUDP clientevocalesUDP.o comun.o $(LIBRARIES)

servidorvocalesUDP: servidorvocalesUDP.o comun.o
	$(CC) $(CFLAGS) -o servidorvocalesUDP servidorvocalesUDP.o comun.o $(LIBRARIES)

clientevocalesUDP.o: clientevocalesUDP.c
	$(CC) $(CFLAGS) -c clientevocalesUDP.c

servidorvocalesUDP.o: servidorvocalesUDP.c
	$(CC) $(CFLAGS) -c servidorvocalesUDP.c

comun.o: comun.c
	$(CC) $(CFLAGS) -c comun.c

# objetivo para comprimir los fuentes tal y como se piden para la entrega
tarea:
	tar cvzf "vocalesUDP_${LOGNAME}.tar.gz" *.c *.h ?akefile

# objetivo para limpiar: borra todo lo generado
clean:
	-rm clientevocalesUDP clientevocalesUDP.o servidorvocalesUDP servidorvocalesUDP.o comun.o "vocalesUDP_${LOGNAME}.tar.gz"
