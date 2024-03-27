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
TRG=10

data="$(while read file; do
    cat "$file" \
        | tr ' ' '\n' \
        | grep -E '^[a-z]+$'
done < <(find "$SRC" -type f) \
    | sort \
    | uniq -c \
    | sed 's/^[[:space:]]*//g' \
    | sort -t ' ' -k 1 -nr)"

cnt_files=$(find $SRC -type f | wc -l)
cnt=0
while read line; do
    if [[ $cnt -eq $TRG ]]; then
        break
    fi

    mfiles=0
    while read file; do
        word="$(echo "$line" | cut -d ' ' -f2)"
        if cat "$file" \
            | tr ' ' '\n' \
            | grep -E '^[a-z]+$' \
            | grep -qs "$word"; then
                    mfiles=$((mfiles + 1))
        fi

        if [[ $mfiles -ge 3 && $mfiles -ge $(( cnt_files / 2 )) ]]; then
            echo "$word"
        fi

    done < <(find "$SRC" -type f)

    cnt=$((cnt + 1))
done < <(echo "$data")
