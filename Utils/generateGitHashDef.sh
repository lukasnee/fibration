#!/bin/bash

source Utils/config.sh

print_usage() {
    printf "usage: $0 <outputPath>\n"
}

[[ $# -eq 0 ]] && (print_usage; exit 1)

gitHash=$(git describe --always --dirty --match 'NOT A TAG')
printf "${colorYellow}git hash: ${colorPurple}$gitHash${colorReset}\n"
printf "GITHASH($gitHash)" > $1
exit 0
