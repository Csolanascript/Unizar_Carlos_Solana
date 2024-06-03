#!/bin/bash
#870984, Simon Gayan, Daniel, M, 3, A
#872815, Solana Melero, Carlos, M, 3, A
IFS= 
echo -n "Introduzca una tecla: "
read palabra
tecla="${palabra:0:1}"
case "$tecla" in
    [a-zA-Z])
	echo "$tecla es una letra"
        ;;
    [0-9])
    echo "$tecla es un numero"
        ;;
    *)
	echo "$tecla es un caracter especial"
esac
