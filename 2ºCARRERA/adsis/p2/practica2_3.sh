#!/bin/bash
#870984, Simon Gayan, Daniel, M, 3, A
#872815, Solana Melero, Carlos, M, 3, A

if test $# = 1
	then 
	if test -f "$1"
		then
		chmod ug+x "$1"
		modoFin=$(stat -c %A "$1")
		echo $modoFin
	else
		echo "$1 no existe"
	fi
else
	echo "Sintaxis: practica2_3.sh <nombre_archivo>"
fi
