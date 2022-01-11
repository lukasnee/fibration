#!/bin/bash

source Utils/config.sh

if [ $# -eq 0 ]; then 
    printf "${colorYellow}usage: $0 <device>\n"
    exit -1;
fi

DEVICE=$1

[[ ! -f $(command -v minicom) ]] && (printf "${colorRed}no 'minicom' for serial communication\n"; exit -1)

minicom --baudrate=$CONFIG_FIBCOM_BAUDRATE --device=$DEVICE -c on
