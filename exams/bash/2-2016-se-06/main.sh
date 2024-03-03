#!/bin/bash

if [[ "$#" -ne 1 ]];then
    echo "provide 1 arg"
    exit 99
fi

if ! [[ -f "$1" ]];then
    echo "1 arg has to be a file"
    exit 99
fi

cat "$1" | tr -d '-' | sed 's/ \+/ /g' | cut -d ' ' -f3- | awk 'BEGIN{c=0}{c++; printf "%d. %s\n", c, $0}' | sort -t ' ' -k 2 
