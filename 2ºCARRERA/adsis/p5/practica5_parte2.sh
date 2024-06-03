#!/bin/bash

if [ $# -ne 1 ]
then
        echo "Es necesario un parametro especificando la IP"
        exit 1
fi

ssh as@"$1" "sudo sfdisk -s && sudo sfdisk -l && sudo df -hT | grep -v -e 'udev' -e 'tmpfs'"