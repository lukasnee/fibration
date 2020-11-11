#!/bin/bash
set -e
if [ -d "build" ]; then
    rm -r build
fi
mkdir build && cd build
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j $(nproc)
