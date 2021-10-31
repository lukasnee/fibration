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
    printf "\t${colorYellow}-r\t ${colorGreen}to reset after flashing\n"
    printf "\t${colorYellow}-b\t ${colorGreen}to prebuild the project target\n"
    exit -1;
fi

PROJECT=$1
TARGET=$2
RESET_FLAG=$3
BUILD_FLAG=$4

[[ ! -f $(command -v st-flash) ]] && 
    printf "${colorRed}no 'st-flash' executable\n" && exit -2

[[ $TARGET == "Debug" || $TARGET == "Release" ]] || 
    ( printf "${colorRed}bad target\n"; exit -3 )

[[ $BUILD_FLAG == "-b" ]] && $SCRIPT_DIR/build.sh $TARGET
[ $? -ne 0 ] && exit -4

PROJECT_DIR="$BUILD_DIR/$TARGET/Modules/$PROJECT"
[[ ! -d $PROJECT_DIR ]] && 
    ( printf "${colorRed}project ${colorPurple}$PROJECT ${colorRed}does not exist or not built\n"; exit -5 )

BINARY_PATH="$PROJECT_DIR/$PROJECT.bin"
[[ ! -f $BINARY_PATH ]] && 
    ( printf "${colorRed}project ${colorPurple}$PROJECT ${colorRed}binary does not exist or not built\n"; exit -6 )

 $SCRIPT_DIR/info.sh
 [ $? -ne 0 ] && exit -7

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
