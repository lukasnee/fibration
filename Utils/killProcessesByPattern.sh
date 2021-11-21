#!/bin/bash

set -e 

# ANSI colors
colorReset='\033[0m'
colorBlack='\033[0;30m'
colorRed='\033[0;31m'
colorGreen='\033[0;32m'
colorYellow='\033[0;33m'
colorBlue='\033[0;34m'
colorPurple='\033[0;35m'
colorCyan='\033[0;36m'
colorWhite='\033[0;37m'

PROCESS_NAME_PATTERN=$1

PIDS=$(pgrep $PROCESS_NAME_PATTERN | tr '\n' ' ')

if [[ ! -z $PIDS ]]; then
    # printf "${colorYellow}processes found:\n${colorGreen}$(ps -p $PIDS)${colorYellow}\n"
    printf "${colorYellow}killing all '$PROCESS_NAME_PATTERN' name pattern processes: ${colorRed}$PIDS${colorReset}\n" 
    kill -s 15 $PIDS
fi
