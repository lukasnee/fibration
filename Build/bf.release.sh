#!/bin/bash
./build.release.sh
./flash-to-stm32.sh Release/Tests/freertosBlinkyTest/freertosBlinkyTest.bin
