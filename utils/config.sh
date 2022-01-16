#!/bin/bash

set -e

# General:

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# ANSI colors
ansiColorReset='\e[0m'
ansiColorBlack='\e[0;30m'
ansiColorRed='\e[0;31m'
ansiColorGreen='\e[0;32m'
ansiColorYellow='\e[0;33m'
ansiColorBlue='\e[0;34m'
ansiColorPurple='\e[0;35m'
ansiColorCyan='\e[0;36m'
ansiColorWhite='\e[0;37m'

ansiThicknessBold='\e[1m'
ansiThicknessReset='\e[22m'

# Project level:

MODULES="modules"

BUILD_DIR=".build" # cross compile
CMAKE_BUILD_CONFIG_PATH=$(realpath "$SCRIPT_DIR/cmakeToolchainFiles/arm-none-eabi-gcc.cmake")
TESTS_DIR="tests"
CMAKE_TESTS_CONFIG_PATH=$(realpath "$SCRIPT_DIR/cmakeToolchainFiles/gcc.cmake")
UNIT_TEST_EXE_PATH="$BUILD_DIR/tests/fibrationUnitTests"

CONFIG_FIBCOM_BAUDRATE=921600