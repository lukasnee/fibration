#!/bin/bash

set -e

# ANSI colors
colorReset='\033[0m'
colorBlack='\033[0;30m'
colorRed='\033[0;31m'
colorGreen='\033[0;32m'
colorYellow='\033[0;33m'
colorBlue='\033[0;34m'
colorcolorPurple='\033[0;35m'
colorCyan='\033[0;36m'
colorWhite='\033[0;37m'

# configs
BUILD_DIR="Build"
CMAKE_CONFIG_PATH="arm-none-eabi-gcc.cmake"
CMAKE_CONFIG_FULL_PATH=$(realpath $CMAKE_CONFIG_PATH)

[ $# -eq 0 ] && ( printf "${colorYellow}usage: $0 <PROJECT> <Debug|Release> [-r]\n"; exit -1; )

PROJECT=$1
TARGET=$2
REBUILD_FLAG=$3

PROJECT_DIR="$BUILD_DIR/$TARGET/Modules/$PROJECT"
ELF_PATH="$PROJECT_DIR/$PROJECT"
[[ -f $ELF_PATH ]] && rm $ELF_PATH

[[ ! -z $REBUILD_FLAG && $REBUILD_FLAG == "-r" ]] && ACTION="rebuild" || ACTION="build"

[[ $TARGET == "Debug" || $TARGET == "Release" ]] \
    && printf "${colorYellow}${ACTION}ing ${colorPurple}$TARGET${colorYellow}\n" \
    || ( printf "${colorRed}bad target\n"; exit -2 )

[[ $REBUILD_FLAG == "-r" ]] && Utils/clean.sh $TARGET

printf $colorCyan

TARGET_BUILD_DIR="$BUILD_DIR/$TARGET"
if [ ! -d "$TARGET_BUILD_DIR" ]; then
    mkdir $TARGET_BUILD_DIR --parents
fi

Utils/gitHash.sh gitHash.def

pushd $TARGET_BUILD_DIR
cmake -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_CONFIG_FULL_PATH \
    -DCMAKE_BUILD_TYPE=$TARGET \
    ../../

cmake --build . -- -j $(nproc) \
    && printf "${colorGreen}built successfully !\n" || printf "${colorRed}build failed\n"


popd > /dev/null

rm -f gitHash.def
