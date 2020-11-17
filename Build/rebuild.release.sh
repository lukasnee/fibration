#!/bin/bash
BUILD_DIR=Release
set -e
./clean.release.sh
mkdir ${BUILD_DIR}
cd ${BUILD_DIR}
	cmake -G "Unix Makefiles" \
	-DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake \
	-DCMAKE_BUILD_TYPE=Release \
	../../

cmake --build . -- -j $(nproc)
