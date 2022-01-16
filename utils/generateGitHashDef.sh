#!/bin/bash

set -e

source utils/config.sh

print_usage() {
    printf "usage: $0 <outputPath>\n"
}

[[ $# -eq 0 ]] && (print_usage; exit 1)

gitHash=$(git describe --always --dirty --match 'NOT A TAG')
printf "${ansiColorYellow}git hash: ${ansiColorPurple}$gitHash${ansiColorReset}\n"
printf "GITHASH($gitHash)" > $1
exit 0
