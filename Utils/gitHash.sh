#!/bin/bash
set -e 

if [ $# -eq 0 ]; then 
    printf "${Yellow}usage: $0 <outputPath>\n"
    exit 1
fi

gitHash=$(git describe --always --dirty --match 'NOT A TAG')
echo git hash: $gitHash
echo "GITHASH($gitHash)" > $1
exit 0
