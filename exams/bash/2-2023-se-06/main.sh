#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    ecit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    ecit 99
fi

PHOT="$1"
LIB="$2"

data="$(while read photo; do
    mtime="$(stat -c '%y' "$photo" \
        | cut -d '.' -f1 \
        | tr ' ' '_')" 

    echo "$(basename $photo) $mtime"
done < <(find "$PHOT" -type f -name *.jpg))"

dates="$(echo "$data" \
    | cut -d ' ' -f2 \
    | sort -n \
    | cut -d '_' -f1 \
    | uniq)"

dir=""
if [[ $(echo "$dates" | wc -l) -eq 1 ]]; then
    dir="$dates"
else
    down_lim="$(echo "$dates" | head -n 1)"
    up_lim="$(echo "$dates" | tail -n 1)"
    dir="${down_lim}_${up_lim}"
fi

mkdir -p "$LIB/$dir"

while read file; do
    p2="$(echo "$file" | cut -d ' ' -f2)"
    nm="$p2.jpg"
    mv "$file" "./$LIB/$dir/$nm"
done < <(echo "$data" | cut -d ' ' -f1)

