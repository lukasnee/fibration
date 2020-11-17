#!/bin/bash
BUILD_DIR=Release
set -e
if [ -d "${BUILD_DIR}" ]; then
    rm -r ${BUILD_DIR}
fi

