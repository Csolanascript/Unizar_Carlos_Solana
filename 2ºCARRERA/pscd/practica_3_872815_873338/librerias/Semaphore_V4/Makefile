#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   octubre 2018
# Coms:   Ejecutar "make" para compilar (o "make -f Makefile")
#         Ejecutar "make clean" para limpiar (borrar lo que genere)
#*****************************************************************

#---------------------------------------------------------
# macros
RM = /bin/rm -f  # para limpiar
CC = g++         # compilador
#---------------------------------------------------------
# opciones compilación y enlazado ("linkado")
CPPFLAGS = -I. -I${SEM_LIB} -O2 -std=c++11 -fmax-errors=1  # opciones compilación
LDFLAGS  = -pthread  # opciones de "linkado"
#---------------------------------------------------------
# vars
EJEC = pruebaSemaforos
#---------------------------------------------------------
# directorio y clase para semáforos
SEM_LIB=.
SEMAPHORE=${SEM_LIB}/Semaphore_V4
#---------------------------------------------------------
all: ${EJEC}
#---------------------------------------------------------
# "linkar"
${EJEC}: ${EJEC}.o ${SEMAPHORE}.o 
	${CC} ${EJEC}.o ${SEMAPHORE}.o -o ${EJEC} ${CPPFLAGS} ${LDFLAGS}

#---------------------------------------------------------
# compilar
${EJEC}.o: ${EJEC}.cpp 
	${CC} -c ${EJEC}.cpp ${CPPFLAGS}

${SEMAPHORE}.o: ${SEMAPHORE}.hpp ${SEMAPHORE}.cpp
	${CC} -c ${SEMAPHORE}.cpp -o ${SEMAPHORE}.o ${CPPFLAGS}

#---------------------------------------------------------
# Cuidado con lo que se pone aquí, que se borra sin preguntar
clean:
	$(RM) ${SEMAPHORE}.o ${EJEC}.o ${EJEC}
