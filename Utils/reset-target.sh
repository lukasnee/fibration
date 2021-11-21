#!/bin/bash

set -e

openocd -f "interface/stlink.cfg" -f "target/stm32f3x.cfg" -c "init;reset;exit"
