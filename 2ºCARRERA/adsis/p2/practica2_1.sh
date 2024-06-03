#!/bin/bash
#870984, Simon Gayan, Daniel, M, 3, A
#872815, Solana Melero, Carlos, M, 3, A

echo -n "Introduzca el nombre del fichero: "
read nomFich
if [ -f "$nomFich" ]
then 
    echo -n "Los permisos del archivo "$nomFich" son: "
    if [ -r "$nomFich" ]
    then
        echo -n "r"
    else 
        echo -n "-"
    fi
    if [ -w "$nomFich" ]
    then
                echo -n "w"
    else
                echo -n "-"
    fi
    if [ -x "$nomFich" ]
    then
                echo "x"
    else
                echo "-"
    fi
else 
    echo "$nomFich" no existe
fi

