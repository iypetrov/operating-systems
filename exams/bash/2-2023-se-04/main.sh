#!/bin/bash

check_equal_files() {
    sha1="$(sha256sum "$1" | cut -d ' ' -f1)"
    sha2="$(sha256sum "$2" | cut -d ' ' -f1)"

    if [[ "$sha1" = "$sha2" ]]; then
        return 0
    fi

    return 1
}

replace_symb_with_file() {
    trg="$1"
    dst="$2"

    rm -rf "$dst"
    ln -s "$trg" "$dst"
}

if [[ $# -ne 1 ]]; then
    echo "provide 1 arg"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

SRC="$1"
visited=""
data="$(while read file; do
    while read line; do
        if [[ "$file" != "$line" ]]; then
            if check_equal_files "$file" "$line"; then
                if ! echo "$visited" | tr ' ' '\n' | grep -qs "$file"; then
                    echo "$file" "$line"
                    visited="$(echo "$visited$line ")"
                fi
            fi
        fi
    done < <(find "$SRC" -type f)
done < <(find "$SRC" -type f))"

while read line; do
    trg="$(echo "$line" | cut -d ' ' -f1 | xargs realpath)"
    dst="$(echo "$line" | cut -d ' ' -f2 | xargs realpath)"
    replace_symb_with_file "$trg" "$dst"

done < <(echo "$data")

tot_couples="$(echo "$data" \
    | awk 'END {print NR}')"

tot_bytes="$(echo "$data" \
    | cut -d ' ' -f1 \
    | xargs realpath \
    | xargs stat -c '%s' \
    | awk 'BEGIN{cnt=0} {cnt = cnt + $1} END{printf "%d\n", cnt}')"

echo "Freed memory: $tot_bytes"
echo "Number of deduplucated file groups: $tot_couples"
