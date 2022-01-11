#!/bin/bash

source utils/config.sh

UNIT_TEST_EXE_PATH="$BUILD_DIR/tests/tests"

if [[ -f $UNIT_TEST_EXE_PATH ]]; then

printf \
"${ansiColorGreen}${ansiThicknessBold}"\
"Running unit tests...\n"\
"===============================================================================\n"\
"${ansiThicknessReset}${ansiColorReset}"

./$UNIT_TEST_EXE_PATH $@ # --durations yes
fi
