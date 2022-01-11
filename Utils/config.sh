#!/bin/bash

set -e

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

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

MODULES="Modules"

BUILD_DIR="Build" # cross compile
CMAKE_BUILD_CONFIG_PATH=$(realpath "$SCRIPT_DIR/cmakeToolchainFiles/arm-none-eabi-gcc.cmake")
TESTS_DIR="Tests"
CMAKE_TESTS_CONFIG_PATH=$(realpath "$SCRIPT_DIR/cmakeToolchainFiles/gcc.cmake")

CONFIG_FIBCOM_BAUDRATE=921600