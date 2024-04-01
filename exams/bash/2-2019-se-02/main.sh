#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "provide at least 1 arg"
    exit 99
fi

c=1
N=10
if [[ "$1" = "-n" ]]; then
    if [[ $# -eq 1 ]]; then
        echo "if the 1 arg is -n you should provide 2 arg that is a number"
        exit 99
    fi

    if ! [[ "$2" =~ ^[0-9]+$ ]]; then
        echo "if the 1 arg is -n you should provide 2 arg that is a number"
        exit 99
    fi

    N=$2
    c=$((c + 1))
fi

files="$(echo "$@" | cut -d ' ' -f${c}-)"

data="$(while read file; do
    fname="$(echo "$file" \
        | xargs basename \
        | rev \
        | cut -d '.' -f2- \
        | rev)"

    while read line; do
        fdate="$(echo "$line" | cut -d ' ' -f1,2)"
        fcnt="$(echo "$line" | cut -d ' ' -f3-)"

        echo "$fdate $fname $fcnt"
    done < <(cat "$file" | tail -n ${N})
done < <(echo "$files" | tr ' ' '\n'))"

echo "$data" | awk '{$1=$1}1' | sort -t ' ' -k1.1,1.4nr -k1.6,1.7nr -k1.9,1.10nr -k2.1,2.2nr -k2.4,2.6nr -k2.7,2.9n

