#!/bin/bash
#870984, Simon Gayan, Daniel, M, 3, A
#872815, Solana Melero, Carlos, M, 3, A

echo "Introduzca el nombre de un directorio: "
read directorio

nArchivos=0
nDirectorios=0
if [ -d "$directorio" ]; 
then
	echo -n "El numero de ficheros y directorios en $directorio es de "
	for archivo in "$directorio"/*
	do
		if [ -d "$archivo" ]
		then
			((nDirectorios++))
		elif [ -f "$archivo" ]
		then
			((nArchivos++))
		fi
	done
	echo "$nArchivos y $nDirectorios, respectivamente"
else
	echo  "$directorio no es un directorio"
fi

