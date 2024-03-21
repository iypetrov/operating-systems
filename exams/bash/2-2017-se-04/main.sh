#!/bin/bash

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "provide 1 or 2 args"
    exit 99
fi

DIR="$1"
trg=""

if [[ $# -eq 2 ]]; then
    trg="$(realpath "$2")"
fi

cnt=0
while read line; do
    file="$(echo "$line")"
    if [[ -e "$file" ]]; then
        if [[ "$trg" = "" ]]; then
            echo "$file -> $line"
        else
            echo "$file -> $line" > "$trg"
        fi
    else
        cnt=$((cnt+1))
    fi
done < <(find "$DIR" -type l)
echo "$cnt"
