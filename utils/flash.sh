#!/bin/bash

set -e

source utils/config.sh

print_usage() {
    printf "usage: 
  -p <project>          module name
  -t <release|debug>    build type
  -b                    prebuild project before flashing
  -h                    show this help
"
}

PROJECT=""
BUILD_TYPE="release"
PREBUILD_FLAG=0
[[ $# -eq 0 ]] && (print_usage; exit 1)
while getopts 'p:t:bh' flag; do
  case "${flag}" in
    p) 
        PROJECT="${OPTARG}"
        [[ -d "$MODULES/$PROJECT" ]] || ( printf "${ansiColorRed}no such project in Modules directory\n"; exit -1 ) 
        ;;
    t) 
        BUILD_TYPE="${OPTARG}"
        [[ $BUILD_TYPE == "debug" || $BUILD_TYPE == "release" ]] || ( printf "${ansiColorRed}invalid build type\n"; exit -1 )
        ;;
    b) 
        PREBUILD_FLAG=1
        ;;
    *|h) 
        print_usage; exit 1 
        ;;
  esac
done

[[ -z $PROJECT ]] && ( printf "${ansiColorRed}project unspecified\n"; exit -1 )

# build structure
PROJECT_DIR="$BUILD_DIR/$BUILD_TYPE/$MODULES/$PROJECT"

EXEC_TO_CHECK="openocd"
[[ ! -f $(command -v $EXEC_TO_CHECK) ]] && 
    printf "${ansiColorRed}no '$EXEC_TO_CHECK' executable\n" && exit -1

[[ $PREBUILD_FLAG -eq 1 ]] && (utils/build.sh -p $PROJECT -t $BUILD_TYPE || exit -1 )

ELF_PATH="$PROJECT_DIR/$PROJECT"
[[ ! -f $ELF_PATH ]] && 
    ( printf "${ansiColorRed}project ${ansiColorPurple}$PROJECT ${ansiColorRed}ELF file does not exist or not built\n"; exit -1 )

printf "${ansiColorYellow}flashing image from ELF: ${ansiColorPurple}$ELF_PATH\n"

openocd -c "set BUILD_TYPE $BUILD_TYPE" -f "$MODULES/$PROJECT/openocd.cfg" -c "exit" \
    && printf "${ansiColorGreen}flashed successfully !\n" || printf "${ansiColorRed}flashing failed\n" 
