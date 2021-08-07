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

#  configs
BUILD_DIR="Build"
TARGET_BUILD_DIR="$BUILD_DIR/$TARGET"

[ $# -eq 0 ] && ( printf "${Yellow}usage: $0 <Debug|Release>\n"; exit -1; )

TARGET=$1

[[ $TARGET == "Debug" || $TARGET == "Release" ]] || ( printf "${Red}bad target\n"; exit -2 )

TARGET_BUILD_DIR="$BUILD_DIR/$TARGET"
printf "${Yellow}cleaning ${Purple}$TARGET_BUILD_DIR\n"
if [ -d "$TARGET_BUILD_DIR" ]; then
    rm -r $TARGET_BUILD_DIR
fi
