#!/bin/bash

if [[ "$#" -ne 1 ]]; then
    echo "provide 1 arg"
    exit 99
fi

if [[ ! -d "$1" ]]; then
    echo "provide dir"
    exit 99
fi

path="$1"
 
whlile read line; do
    if [[ ! -e $(readline "$line")]]; then
        echo "$line"
    fi
done < <(find "$path" -type l) 
