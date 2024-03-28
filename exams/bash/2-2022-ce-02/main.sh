#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "provide at least 1 arg" 
    exit 99
fi

if [[ $1 = "DEVICE" ]]; then
    echo "1 arg can not be a DEVICE" 
    exit 99
fi

TRG="$1"

index="$(cat "input.txt" \
    | awk -v TRG="$TRG" '{cnt++; if (TRG == $1) { printf "%d %s\n", cnt, $0}}' \
    | cut -d ' ' -f1)"
line="$(cat "input.txt" \
    | sed -n "${index}p" \
    | awk -v DIS="disabled" '{printf "%s\t%s\t%s\t%s\n", $1, $2, DIS, $4}')"

data="$(awk -v NUM="$index" -v LINE="$line" '{cnt++; if(cnt==NUM) {print LINE} else {printf "%s\t%s\t%s\t%s\n", $1, $2, $3, $4}}' input.txt)"
echo "$data" > input.txt

