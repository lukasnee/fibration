#!/bin/bash
DEBUG_DIR=Debug
set -e
if [ ! -d "${DEBUG_DIR}" ]; then
    mkdir ${DEBUG_DIR}
fi
cd ${DEBUG_DIR}
cmake -G "Unix Makefiles" \
	-DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake \
	-DCMAKE_BUILD_TYPE=Debug \
	../../
	
cmake --build . -- -j $(nproc)
