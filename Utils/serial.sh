#!/bin/bash
set -e 

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Regular Colors
Color_Off='\033[0m'       # Text Reset
Black='\033[0;30m'        # Black
Red='\033[0;31m'          # Red
Green='\033[0;32m'        # Green
Yellow='\033[0;33m'       # Yellow
Blue='\033[0;34m'         # Blue
Purple='\033[0;35m'       # Purple
Cyan='\033[0;36m'         # Cyan
White='\033[0;37m'        # White

# configs
BUILD_DIR="Build"

if [ $# -eq 0 ]; then 
    printf "${Yellow}usage: $0 <device>\n"
    exit -1;
fi

DEVICE=$1

[[ ! -f $(command -v minicom) ]] && printf "${Red}no 'minicom' for serial communication\n" && exit -3

minicom --baudrate=921600 --device=$DEVICE -c on
