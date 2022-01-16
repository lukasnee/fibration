#!/bin/bash

set -e

source utils/config.sh

PROCESS_NAME_PATTERN=$1

PIDS=$(pgrep $PROCESS_NAME_PATTERN | tr '\n' ' ')

if [[ ! -z $PIDS ]]; then
    printf "${ansiColorYellow}killing all '$PROCESS_NAME_PATTERN' name pattern processes: ${ansiColorRed}$PIDS${ansiColorReset}\n" 
    kill -s 15 $PIDS
fi
