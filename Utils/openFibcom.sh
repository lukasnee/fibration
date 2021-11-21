#!/bin/bash
set -e 

# ANSI colors
colorReset='\033[0m'
colorBlack='\033[0;30m'
colorRed='\033[0;31m'
colorGreen='\033[0;32m'
colorYellow='\033[0;33m'
colorBlue='\033[0;34m'
colorcolorPurple='\033[0;35m'
colorCyan='\033[0;36m'
colorWhite='\033[0;37m'

if [ $# -eq 0 ]; then 
    printf "${colorYellow}usage: $0 <device>\n"
    exit -1;
fi

DEVICE=$1

FIBCOM_BAUDRATE=921600

[[ ! -f $(command -v minicom) ]] && printf "${colorRed}no 'minicom' for serial communication\n" && exit -3

minicom --baudrate=$FIBCOM_BAUDRATE --device=$DEVICE -c on
