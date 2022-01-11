#!/bin/bash

source utils/config.sh

PROCESS_NAME_PATTERN=$1

PIDS=$(pgrep $PROCESS_NAME_PATTERN | tr '\n' ' ')

if [[ ! -z $PIDS ]]; then
    printf "${colorYellow}killing all '$PROCESS_NAME_PATTERN' name pattern processes: ${colorRed}$PIDS${colorReset}\n" 
    kill -s 15 $PIDS
fi
