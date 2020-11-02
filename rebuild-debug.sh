#!/bin/bash
set -e
rm -r build
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j $(nproc)
