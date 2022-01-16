#!/bin/bash

set -e

source utils/config.sh

print_usage() {
    printf "usage: 
  -a                    all
  -p <project>          module name
  -t <release|debug>    build type
  -r                    rebuild
  -h                    show this help
"
}
all=0
PROJECT=""
BUILD_TYPE="release"
[[ $# -eq 0 ]] && (print_usage; exit 1)
while getopts 'ap:t:h' flag; do
  case "${flag}" in
    a)
        all=1 ;; 
    p) 
        PROJECT="${OPTARG}"
        [[ -d "$MODULES/$PROJECT" ]] || ( printf "${ansiColorRed}no such project in Modules directory\n"; exit -1 ) 
        ;;
    t) 
        BUILD_TYPE="${OPTARG}"
        [[ $BUILD_TYPE == "debug" || $BUILD_TYPE == "release" ]] || ( printf "${ansiColorRed}invalid build type\n"; exit -1 )
        ;;
    *|h) 
        print_usage; exit 1 
        ;;
  esac
done

# build structure
if [[ all -gt 0 ]]; then 
    DIR_TO_DELETE="$BUILD_DIR"
else
    DIR_TO_DELETE="$BUILD_DIR/$BUILD_TYPE/$MODULES/$PROJECT"
fi

printf "${ansiColorYellow}cleaning ${ansiColorPurple}$DIR_TO_DELETE\n"; 
[[ -d $DIR_TO_DELETE ]] && rm -r $DIR_TO_DELETE
