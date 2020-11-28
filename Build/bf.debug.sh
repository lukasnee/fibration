#!/bin/bash
set -e
./build.debug.sh
./flash-to-stm32.sh Debug/Tests/freertosBlinkyTest/freertosBlinkyTest.bin
