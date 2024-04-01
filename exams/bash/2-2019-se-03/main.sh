#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "provide 1 arg"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

SRC="$1"
TRG="/extracted"

tmp="$(mktemp)"
h="$(pwd)"
cd "$tmp"
while read file; do
    if basename "$file" | grep -qs -E '^[^_]+_report-[0-9]{9}\.tgz'; then
        name="$(echo "$file" | cut -d '_' -f1)"
        tmst="$(echo "$file" cut -d '-' -f2)"
        # check if element already exist and is not modified
        if ! find "$TRG" -type f | xargs basename | grep -qs "${name}_${tmst}.txt";
            continue
        fi

        old_chk="$(sha256sum "$TRG/${name}_${tmst}.txt" )"

        # add new element 
        arch="$(tar -xzvf "$file")"
        if echo "$arch" | xargs basename | grep -qs -F "meow.txt"; then
            trg="$(echo "$arch" | xargs basename | grep -F "meow.txt" | xargs realpath)"    
            new_chk="$(sha256sum "$trg" )"

            if [[ "$old_chk" != "$new_chk" ]]; then
                cp "$trg"  "$TRG/${name}_${tmst}.txt"
            fi
        fi
    fi
done < <(find "$SRC" -type f)
cd "$h"
