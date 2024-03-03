#!/bin/bash

if [[ "$#" -ne 2 ]];then
    echo "provide 2 args"
    exit 99
fi

if ! [[ -f "$1" ]];then
    echo "1 arg should be a file"
    exit 99
fi

if ! [[ -f "$2" ]];then
    echo "2 arg should be a file"
    exit 99
fi

one="$(cat "$1" | cut -d ' ' -f3 | grep -o "$1" | wc -l)"
two="$(cat "$2" | cut -d ' ' -f3 | grep -o "$2" | wc -l)"

file="$2"

if [[ $one -ge $two ]];then
    file="$1"
fi

cat "$file" | cut -d ' ' -f5- | sort
