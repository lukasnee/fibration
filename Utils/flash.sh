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

# configs
BUILD_DIR="Build"

print_usage() {
    printf "${colorYellow}usage: $0 <PROJECT> <Debug|Release> [-b]\n"
    printf "\t${colorYellow}-b\t ${colorGreen}to prebuild the project target\n"
}

if [ $# -eq 0 ]; then
    print_usage
fi

PROJECT=$1
TARGET=$2
FLAG_BUILD=$3

EXEC_TO_CHECK="openocd"
[[ ! -f $(command -v $EXEC_TO_CHECK) ]] && 
    printf "${colorRed}no '$EXEC_TO_CHECK' executable\n" && exit -2

[[ $TARGET == "Debug" || $TARGET == "Release" ]] || 
    ( printf "${colorRed}bad target ${colorPurple}$TARGET\n"; exit -3 )

[[ $FLAG_BUILD == "-b" ]] && (Utils/build.sh $PROJECT $TARGET || exit -4 )

PROJECT_DIR="$BUILD_DIR/$TARGET/Modules/$PROJECT"
[[ ! -d $PROJECT_DIR ]] && 
    ( printf "${colorRed}project ${colorPurple}$PROJECT ${colorRed}does not exist or not built\n"; exit -5 )

ELF_PATH="$PROJECT_DIR/$PROJECT"
[[ ! -f $ELF_PATH ]] && 
    ( printf "${colorRed}project ${colorPurple}$PROJECT ${colorRed}ELF file does not exist or not built\n"; exit -6 )

printf "${colorYellow}flashing image from ELF: ${colorPurple}$ELF_PATH\n"

openocd -c "set TARGET $TARGET" -f "Modules/$PROJECT/openocd.cfg" -c "exit" \
    && printf "${colorGreen}flashed successfully !\n" || printf "${colorRed}flashing failed\n" 
