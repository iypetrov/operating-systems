#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    exit 99 
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99 
fi

if ! [[ -d  "$2" ]]; then
    echo "2 arg should be a dir"
    exit 99
fi

if [[ $(find "$2" -type f | wc -l) -ne 0 ]]; then
    echo "2 arg should be an empty dir"
    exit 99
fi

SRC="$(realpath $1)"
TRG="$(realpath $2)"

rm -rf "$TRG"
cp -r "$SRC" "$TRG"

while read file; do
    if basename "$file" | grep -qs -E '^\..*\.swp$'; then
        rm "$file"
    fi
done < <(find "$TRG" -type f)
