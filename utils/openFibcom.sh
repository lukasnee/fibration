#!/bin/bash

set -e

source utils/config.sh

if [ $# -eq 0 ]; then 
    printf "${ansiColorYellow}usage: $0 <device>\n"
    exit -1;
fi

DEVICE=$1

[[ ! -f $(command -v minicom) ]] && (printf "${ansiColorRed}no 'minicom' for serial communication\n"; exit -1)

minicom --baudrate=$CONFIG_FIBCOM_BAUDRATE --device=$DEVICE --color=on
