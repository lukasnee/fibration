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
CMAKE_CONFIG_PATH="arm-none-eabi-gcc.cmake"
CMAKE_CONFIG_FULL_PATH=$(realpath $CMAKE_CONFIG_PATH)

[ $# -eq 0 ] && ( printf "${Yellow}usage: $0 <Debug|Release> [-r]\n"; exit -1; )

TARGET=$1
REBUILD_FLAG=$2

[[ ! -z $REBUILD_FLAG && $REBUILD_FLAG == "-r" ]] && ACTION="rebuild" || ACTION="build"

[[ $TARGET == "Debug" || $TARGET == "Release" ]] \
    && printf "${Yellow}${ACTION}ing ${Purple}$TARGET${Yellow}\n" \
    || ( printf "${Red}bad target\n"; exit -2 )

[[ $REBUILD_FLAG == "-r" ]] && $SCRIPT_DIR/clean.sh $TARGET

printf $Cyan

TARGET_BUILD_DIR="$BUILD_DIR/$TARGET"
if [ ! -d "$TARGET_BUILD_DIR" ]; then
    mkdir $TARGET_BUILD_DIR --parents
fi

sh $SCRIPT_DIR/gitHash.sh gitHash.def

cd $TARGET_BUILD_DIR
cmake -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_CONFIG_FULL_PATH \
    -DCMAKE_BUILD_TYPE=$TARGET \
    ../../

cmake --build . -- -j $(nproc)

cd -

rm -f gitHash.def
