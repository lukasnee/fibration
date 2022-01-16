#!/bin/bash

set -e

source utils/config.sh

print_usage() {
    printf "usage: 
  -r                    rebuild
  -h                    show this help
"
}

BUILD_TYPE="tests"
REBUILD_FLAG=0
while getopts 'rh' flag; do
  case "${flag}" in
    r) 
        REBUILD_FLAG=1
        ;;
    *|h) 
        print_usage; exit 1 
        ;;
  esac
done

# clean build if rebuild flag
ACTION="build"
[[ $REBUILD_FLAG -eq 1 ]] && (utils/clean.sh -t $BUILD_TYPE; ACTION="rebuild")

printf "${ansiColorYellow}${ACTION}ing ${ansiColorPurple}$BUILD_TYPE${ansiColorYellow}\n" 

# structure build types
BUILD_TYPE_DIR="$BUILD_DIR/$BUILD_TYPE"
[[ ! -d "$BUILD_TYPE_BUILD_DIR" ]] && mkdir $BUILD_TYPE_DIR --parents

pushd $BUILD_TYPE_DIR > /dev/null

printf $ansiColorCyan
cmake -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TESTS_CONFIG_PATH \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    ../../$TESTS_DIR

cmake --build . -- -j $(nproc) \
    && printf "${ansiColorGreen}built successfully !\n" || printf "${ansiColorRed}build failed\n"

popd > /dev/null
