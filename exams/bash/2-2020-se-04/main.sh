#!/bin/bash

get_albums() {
    alb="$(echo "$1" \
        | grep -o -E '\(.*\)' \
        | tr ' ' '\n' \
        | tr -d '(' \
        | tr -d ')' \
        | tail -n 1 \
        | tee)"

    if [[ -z "$alb" ]]; then
        echo "misc"
        return 0
    fi

    echo "$alb"
}

filt_pht () {
    pht="$(echo "$1" \
        | sed -E 's/\(.*\)//g' \
        | sed 's/[[:space:]]\+/ /g' \
        | tee)"

    echo "$pht"
}

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

SRC="$(realpath "$1")"
DST="$(realpath "$0" | rev | cut -d '/' -f 2- | rev)/$2"

files="$(find "$SRC" -type f -name *.jpg)"

while read file; do
    cfile="$(echo "$file" \
        | tr -d \' \
        | rev \
        | cut -d '/' -f1 \
        | rev \
        | cut -d '.' -f1 \
        | sed 's/^[ \t]*//g' \
        | sed 's/[ \t]*$//g' \
        | sed 's/[ \t]\+/ /g' \
        | tee)"

    alb="$(get_albums "$cfile")"
    pht="$(filt_pht "$cfile")"
    mtime="$(stat -c '%y' "$file" | cut -d ' ' -f1)"
    chk="$(sha256sum "$file" | head -c 16)"

    # create needed dirs + files
    RDST="$(basename "$DST")"

    mkdir -p "$DST/images"
    cp "$file" "$DST/images/$chk.jpg"
    
    mkdir -p "$DST/by-date/$mtime/by-album/$alb/by-title"
    ln -s "$DST/images/$chk.jpg" "$RDST/by-date/$mtime/by-album/$alb/by-title/$pht.jpg"
    
    mkdir -p "$DST/by-date/$mtime/by-title"
    ln -s "$DST/images/$chk.jpg" "$RDST/by-date/$mtime/by-title/$pht.jpg"

    mkdir -p "$DST/by-album/$alb/by-date/$mtime/by-title"
    ln -s "$DST/images/$chk.jpg" "$RDST/by-album/$alb/by-date/$mtime/by-title/$pht.jpg"

    mkdir -p "$DST/by-album/$alb/by-title"
    ln -s "$DST/images/$chk.jpg" "$RDST/by-album/$alb/by-title/$pht.jpg"

    mkdir -p "$DST/by-title"
    ln -s "$DST/images/$chk.jpg" "$RDST/by-title/$pht.jpg"
done < <(echo "$files")

