#!/bin/bash

set -e 

# ANSI colors
colorReset='\033[0m'
colorBlack='\033[0;30m'
colorRed='\033[0;31m'
colorGreen='\033[0;32m'
colorYellow='\033[0;33m'
colorBlue='\033[0;34m'
colorPurple='\033[0;35m'
colorCyan='\033[0;36m'
colorWhite='\033[0;37m'

deviceNameRegex='.*ST-LINK.*'

Utils/killProcessesByPattern.sh openocd

bus=$(lsusb | grep -P -o "(?<=Bus )\d{3}(?= Device \d{3}: ID [[:xdigit:]]{4}:[[:xdigit:]]{4} $deviceNameRegex)")
device=$(lsusb | grep -P -o "(?<= Device )\d{3}(?=: ID [[:xdigit:]]{4}:[[:xdigit:]]{4} $deviceNameRegex)")

devicePath="/dev/bus/usb/$bus/$device"

[[ $bus == "" || $device == "" ]] && printf "not found\n" || Utils/usbreset/usbreset $devicePath

