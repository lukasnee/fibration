#!/bin/bash

set -e

source utils/config.sh

print_usage() {
    printf "usage: 
  -p <project>          module name
  -t <release|debug>    build type
  -r                    rebuild
  -h                    show this help
"
}

PROJECT=""
BUILD_TYPE="release"
REBUILD_FLAG=0
[[ $# -eq 0 ]] && (print_usage; exit 1)
while getopts 'p:t:rh' flag; do
  case "${flag}" in
    p) 
        PROJECT="${OPTARG}"
        [[ -d "$MODULES/$PROJECT" ]] || ( printf "${ansiColorRed}no such project in Modules directory\n"; exit -1 ) 
        ;;
    t) 
        BUILD_TYPE="${OPTARG}"
        [[ $BUILD_TYPE == "release" || $BUILD_TYPE == "debug" ]] || ( printf "${ansiColorRed}invalid build type\n"; exit -1 )
        ;;
    r) 
        REBUILD_FLAG=1
        ;;
    *|h) 
        print_usage; exit 1 
        ;;
  esac
done

[[ -z $PROJECT ]] && ( printf "${ansiColorRed}project unspecified\n"; exit -1 )

PROJECT_DIR="$BUILD_DIR/$BUILD_TYPE/$MODULES/$PROJECT"


# remove ELF to avoid debugging old executable
ELF_PATH="$PROJECT_DIR/$PROJECT"
[[ -f $ELF_PATH ]] && rm $ELF_PATH

# clean build if rebuild flag
ACTION="build"
[[ $REBUILD_FLAG -eq 1 ]] && (utils/clean.sh -p $PROJECT -t $BUILD_TYPE; ACTION="rebuild")

printf "${ansiColorYellow}${ACTION}ing ${ansiColorPurple}$BUILD_TYPE${ansiColorYellow}\n" 

# structure build types
BUILD_TYPE_DIR="$BUILD_DIR/$BUILD_TYPE"
[[ ! -d "$BUILD_TYPE_BUILD_DIR" ]] && mkdir $BUILD_TYPE_DIR --parents

GITHASH_DEF_PATH="gitHash.def"
utils/generateGitHashDef.sh $GITHASH_DEF_PATH
pushd $BUILD_TYPE_DIR > /dev/null

printf $ansiColorCyan
cmake -G "Unix Makefiles" \
    -DCMAKE_TOOLCHAIN_FILE="$CMAKE_BUILD_CONFIG_PATH" \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    ../../

cmake --build . -- -j $(nproc) \
    && printf "${ansiColorGreen}built successfully !\n" || printf "${ansiColorRed}build failed\n"

popd > /dev/null
rm -f $GITHASH_DEF_PATH
