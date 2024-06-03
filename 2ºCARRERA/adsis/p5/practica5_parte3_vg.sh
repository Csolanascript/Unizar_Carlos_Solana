#!/bin/bash

nombreVG="$1"

shift #muevo los parametros uno a la der, asi solo tengo todas las particiones a añadir
sudo vgextend $nombreVG $@

sudo vgs $nombreVG
sudo pvs