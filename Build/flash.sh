#!/bin/bash
set -e 

if [[ $1 -eq "" ]]; then
  echo "usage: ./flash.sh <binary-file>"
  exit 1;
fi

st-flash write $1 0x08000000 
st-flash reset

