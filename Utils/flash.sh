#!/bin/bash
set -e 

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# configs
STM32_IMAGE_BASE=0x08000000

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
    printf "${Yellow}usage: $0 <PROJECT> <Debug|Release> [-r] [-b]\n"
    printf "\t-r\t ${Green}to reset after flashing\n"
    printf "\t-r\t ${Green}to prebuild the project target\n"
    exit -1;
fi

PROJECT=$1
TARGET=$2
RESET_FLAG=$3
BUILD_FLAG=$4

[[ ! -f $(command -v st-flash) ]] && printf "${Red}no 'st-flash' to to flash with\n" && exit -2
[[ ! -f $(command -v st-info) ]] && printf "${Red}no 'st-info' to to get device info with\n" && exit -3

[[ $TARGET == "Debug" || $TARGET == "Release" ]] || ( printf "${Red}bad target\n"; exit -4 )

[[ $BUILD_FLAG == "-b" ]] && $SCRIPT_DIR/build.sh $TARGET
[ $? -ne 0 ] && exit -5

PROJECT_DIR="$BUILD_DIR/$TARGET/Modules/$PROJECT"
[[ ! -d $PROJECT_DIR ]] && ( printf "${Red}project ${Purple}$PROJECT ${Red}does not exist or not built\n"; exit -6 )

BINARY_PATH="$PROJECT_DIR/$PROJECT.bin"
[[ ! -f $BINARY_PATH ]] && ( printf "${Red}project ${Purple}$PROJECT ${Red}binary does not exist or not built\n"; exit -7 )

printf "${Yellow}\nst-info:\n"
printf "  ${Yellow}version  ${Cyan}" ; st-info --version
printf "  ${Yellow}flash    ${Cyan}" ; st-info --flash
printf "  ${Yellow}sram     ${Cyan}" ; st-info --sram
printf "  ${Yellow}descr    ${Cyan}" ; st-info --descr
printf "  ${Yellow}pagesize ${Cyan}" ; st-info --pagesize
printf "  ${Yellow}chipid   ${Cyan}" ; st-info --chipid
printf "  ${Yellow}serial   ${Cyan}" ; st-info --serial
printf "  ${Yellow}serial   ${Cyan}" ; st-info --hla-serial

printf "${Yellow}\nflashing image ${Purple}$BINARY_PATH\n${Cyan}"
st-flash write $BINARY_PATH $STM32_IMAGE_BASE
[ $? -eq 0 ] && printf "${Green}flashed successfully !\n" || printf "${Red}flashing failed\n" 

if [[ $RESET_FLAG == "-r" ]]; then
    printf "${Yellow}\nresetting the flashed ${Purple}"; st-info --descr
    printf "${Cyan}"
    st-flash reset
    [ $? -eq 0 ] && printf "${Green}reset successfully !\n" || printf "${Red}reset failed\n" 
fi

