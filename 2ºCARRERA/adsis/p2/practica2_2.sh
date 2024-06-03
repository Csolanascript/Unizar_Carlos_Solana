#!/bin/bash
#870984, Simon Gayan, Daniel, M, 3, A
#872815, Solana Melero, Carlos, M, 3, A

for param in "$@" 
do
	if test -f "$param"
	then 
		more -e "$param"
	else 
		echo "$param" "no es un fichero"
	fi
done
