#!/bin/bash

set -e

source utils/config.sh

openocd -f "interface/stlink.cfg" -f "target/stm32f3x.cfg" -c "init;reset;exit"
