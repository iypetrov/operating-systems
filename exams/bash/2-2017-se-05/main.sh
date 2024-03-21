#!/bin/bash

is_numb() {
    if ! [[ "$1" =~ ^[0-9]+$ ]]; then
        return 1 
    fi

    return 0 
}

if [[ $# -ne 2 ]]; then
    echo "provide  2 args"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

DIR="$1"
ARCH="$2"

while read line; do
    basename "$line" | sed -n "/vmlinuz-[0-9]*\.[0-9]*\.[0-9]*-$ARCH/p"
done < <(find "$DIR" -maxdepth 1 -type f) | sort -t '-' -k 2 -nr | head -n +1
