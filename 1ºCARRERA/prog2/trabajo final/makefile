
CC = g++
CPPFLAGS = -I. -std=c++11
LDFLAGS =
RM = rm -f

all: mainGSenku

mainGSenku:mainGSenku.o GSenku.o
	${CC} mainGSenku.o GSenku.o -o mainGSenku ${LDFLAGS}
mainGSenku.o: mainGSenku.cpp 
	${CC} -c mainGSenku.cpp ${CPPFLAGS}
GSenku.o: GSenku.cpp GSenku.hpp
	${CC} -c GSenku.cpp ${CPPFLAGS}

clean:
	${RM} GSenku.o mainGSenku.o mainGSenku
