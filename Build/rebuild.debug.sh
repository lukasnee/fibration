#!/bin/bash
DEBUG_DIR=Debug
set -e
./clean.debug.sh
mkdir ${DEBUG_DIR}
cd ${DEBUG_DIR}
	cmake -G "Unix Makefiles" \
	-DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake \
	-DCMAKE_BUILD_TYPE=Debug \
	../../

cmake --build . -- -j $(nproc)
