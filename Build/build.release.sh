#!/bin/bash
BUILD_DIR=Release
set -e
if [ ! -d "${BUILD_DIR}" ]; then
    mkdir ${BUILD_DIR}
fi
cd ${BUILD_DIR}
cmake -G "Unix Makefiles" \
	-DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake \
	-DCMAKE_BUILD_TYPE=Release \
	../../
	
cmake --build . -- -j $(nproc)
