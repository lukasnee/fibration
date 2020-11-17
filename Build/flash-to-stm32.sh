#!/bin/bash
set -e 

if [[ $1 -eq "" ]]; then
  echo "usage: $0 <binary-file>"
  exit 1;
fi

st-flash write $1 0x08000000 
st-flash reset

