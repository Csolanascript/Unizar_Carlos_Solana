#!/bin/bash
OLDIFS=$IFS #lo guardo para luego
IFS=, #los campos los dividimos por comas
read nombreGrupoVolumen nombreVolumenLogico tamanio tipoSistemaFicheros directorioMontaje
rutaLV=$( lvdisplay "$nombreGrupoVolumen/$nombreVolumenLogico" -Co "lv_path" | grep "$nombreGrupoVolumen/$nombreVolumenLogico" ) &> /dev/null
# -Co para mostrar solo esa columna y el grep para que solo muestre la linea en la que aparezca el nombre del vg y del lv, redirijo por si acaso falla porque no existe
if [ -n "$rutaLV" ] #si no es vacio 
then
        echo "Existe,se va a extender"
        lvextend -L $tamanio $rutaLV
        resize2fs $rutaLV
else
        echo "No existe,hace falta crearlo,creando..."
        lvcreate -L $tamanio -n $nombreVolumenLogico $nombreGrupoVolumen
        if [ $? -eq 0 ]; then
        #si ha funcionado la creacion, compruebo si el dir que voy a usar para montar esta creado, si no, lo creo
        if [ ! -d $directorioMontaje ]; then
                mkdir -p $directorioMontaje
                echo "Directorio de montaje creado"
        fi
        rutaLV=$(lvdisplay "$nombreGrupoVolumen/$nombreVolumenLogico" -Co "lv_path" | grep "$nombreGrupoVolumen/$nombreVolumenLogico")
        echo "$vdir $directorioMontaje $tipoSistemaFicheros defaults 0 0" >> /etc/fstab #redirijo este echo al fichero fstab para que se monte al arrancar el sistema
        mkfs.$tipoSistemaFicheros $rutaLV
        #se crea el sistema de ficheros y posteriormente se monta
        mount $rutaLV $directorioMontaje
        fi
fi
IFS=$OLDIFS #restablezco el anterior delimitador