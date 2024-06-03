#!/bin/bash
#870984, Simon Gayan, Daniel, M, 3, A
#872815, Solana Melero, Carlos, M, 3, A


# Encontrar todos los directorios que sigan el patrón binXXX en el directorio raíz del usuario
dirViejo=$(find "$HOME" -maxdepth 1 -type d -name "bin*" -exec stat -c "%Y %n" {} \; | sort -n | head -n 1 | cut -d' ' -f2)

# Si no se encontraron directorios que sigan el patrón binXXX, crear uno nuevo
if [ -z "$dirViejo" ]
then 
	dirNuevo=$(mktemp -d "$HOME/binXXX") 
	echo "Se ha creado el directorio $dirNuevo"
else 
	dirNuevo="$dirViejo" 
fi

echo "Directorio destino de copia: $dirNuevo"
ndir=0

for file in *; do
	if [ -x "$file" ]; then
		if [ -f "$file" ]; then
			cp "$file" "$dirNuevo"
			echo "./$file ha sido copiado a $dirNuevo"
			((ndir++))
		fi
	fi
done

if [ $ndir = 0 ]; then
	echo "No se ha copiado ningun archivo"
elif [ $ndir = "1" ]
then
        echo "Se ha copiado $ndir archivo"
else
        echo "Se han copiado $ndir archivos"
fi








