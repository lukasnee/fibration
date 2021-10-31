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

if [[ $1 == "-h" ]]; then 
    printf "${colorYellow}usage: $0 [-h][-p]\n"
    printf "\t${colorYellow}-h\t ${colorGreen}to show this help\n"
    printf "\t${colorYellow}-p\t ${colorGreen}to probe for st-link\n"
    exit -1;
fi

PROBE=$1

# handle leftover 'st-util' (gdb server) processes
ST_UTIL_PIDS=$(ps aux | grep -P 'st-util' | awk '{print $2}' | head -n -1 | tr '\n' ' ')
if [[ ! -z $ST_UTIL_PIDS ]]; then
    printf "${colorYellow}forcefully killing all 'st-util' processes, PIDS: ${colorRed}$ST_UTIL_PIDS${colorReset}\n" 
    kill -s 15 $ST_UTIL_PIDS
fi

[[ ! -f $(command -v st-info) ]] && 
    printf "${colorRed}no 'st-info' executable\n" && exit -1

printf "\n"

trap "printf \"${colorRed}maybe ST-LINK is unplugged ?\n${colorReset}\"" EXIT
if [[ $PROBE == "-p" ]]; then 
    st-info --probe
else
    FMT="  ${colorYellow}%-16s${colorCyan}"
    printf $FMT "version"   ; st-info --version
    printf $FMT "serial"    ; st-info --serial
    printf $FMT "flash"     ; st-info --flash
    printf $FMT "pagesize"  ; st-info --pagesize
    printf $FMT "sram"      ; st-info --sram
    printf $FMT "chipid"    ; st-info --chipid
    printf $FMT "descr"     ; st-info --descr
fi
trap EXIT

printf "\n"
