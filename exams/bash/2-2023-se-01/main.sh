#!/bin/bash

build_blur () {
    stars="$(seq $(($1 - 1)) | sed -E 's/^[0-9]*/\*/g' | tr -d '\n' )"
    echo "$stars"
}

pick_stars() {
    word="$1"
    bllist="$2"
    filtered="$3"
    pos="$(cat "$bllist" | grep -n "$word" | head -n 1 | cut -d ':' -f1)"
    echo "$filtered" | sed -n "${pos}p"
}

BLACKLIST="$1"
SRC="$2"
FILTERED="$(while read line; do
    cnt=$(echo "$line" | wc -m)
    build_blur $cnt
done < <(cat "$BLACKLIST"))"

while read file; do
    while read bl; do
        strs=$(pick_stars "$bl" "$BLACKLIST" "$FILTERED")
        sed -i "s/\b${bl}\b/${strs}/Ig" "$file"
    done < <(cat "$BLACKLIST")
done < <(find "$SRC" -type f -name '*.txt')
