#!/bin/bash
DEBUG_DIR=Debug
set -e
if [ -d "${DEBUG_DIR}" ]; then
    rm -r ${DEBUG_DIR}
fi

