#!/bin/bash
BUILD_DIR=Debug
set -e
if [ -d "${BUILD_DIR}" ]; then
    rm -r ${BUILD_DIR}
fi

