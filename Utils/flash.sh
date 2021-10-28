#!/bin/bash

# configs
STM32_IMAGE_BASE=0x08000000

set -e 

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

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

# configs
BUILD_DIR="Build"

if [ $# -eq 0 ]; then 
    printf "${colorYellow}usage: $0 <PROJECT> <Debug|Release> [-r] [-b]\n"
    printf "\t-r\t ${colorGreen}to reset after flashing\n"
    printf "\t-r\t ${colorGreen}to prebuild the project target\n"
    exit -1;
fi

PROJECT=$1
TARGET=$2
RESET_FLAG=$3
BUILD_FLAG=$4

[[ ! -f $(command -v st-flash) ]] && 
    printf "${colorRed}no 'st-flash' to to flash with\n" && exit -2
[[ ! -f $(command -v st-info) ]] && 
    printf "${colorRed}no 'st-info' to to get device info with\n" && exit -3

[[ $TARGET == "Debug" || $TARGET == "Release" ]] || 
    ( printf "${colorRed}bad target\n"; exit -4 )

[[ $BUILD_FLAG == "-b" ]] && $SCRIPT_DIR/build.sh $TARGET
[ $? -ne 0 ] && exit -5

PROJECT_DIR="$BUILD_DIR/$TARGET/Modules/$PROJECT"
[[ ! -d $PROJECT_DIR ]] && 
    ( printf "${colorRed}project ${colorPurple}$PROJECT ${colorRed}does not exist or not built\n"; exit -6 )

BINARY_PATH="$PROJECT_DIR/$PROJECT.bin"
[[ ! -f $BINARY_PATH ]] && 
    ( printf "${colorRed}project ${colorPurple}$PROJECT ${colorRed}binary does not exist or not built\n"; exit -7 )

printf "${colorYellow}\nst-info:\n"

trap "printf \"${colorRed}maybe ST-LINK is unplugged ?\n${colorReset}\"" EXIT

FMT="  ${colorYellow}%-16s${colorCyan}"
printf $FMT "version"   ; st-info --version
printf $FMT "flash"     ; st-info --flash
printf $FMT "sram"      ; st-info --sram
printf $FMT "descr"     ; st-info --descr
printf $FMT "pagesize"  ; st-info --pagesize
printf $FMT "chipid"    ; st-info --chipid
printf $FMT "serial"    ; st-info --serial
printf $FMT "hla-serial"; st-info --hla-serial

printf "\n"
printf "${colorYellow}flashing image ${colorPurple}$BINARY_PATH\n"

printf "${colorCyan}"
st-flash write $BINARY_PATH $STM32_IMAGE_BASE

[ $? -eq 0 ] && 
    printf "${colorGreen}flashed successfully !\n" || printf "${colorRed}flashing failed\n" 

if [[ $RESET_FLAG == "-r" ]]; then
    printf "\n"
    printf "${colorYellow}resetting the flashed ${colorPurple}"; st-info --descr
    printf "${colorCyan}"
    st-flash reset
    [ $? -eq 0 ] && 
        printf "${colorGreen}reset successfully !\n" || printf "${colorRed}reset failed\n" 
fi
