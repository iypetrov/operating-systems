#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    exit 99
fi

if [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

SRC="$1"
STAR_TYPE="$2"

CONSTELLATION="$(while read line; do
    type="$(echo "$line" | cut -d ' ' -f2)"
    if [[ "$type" = "$STAR_TYPE" ]]; then
        echo "$line"
    fi
done < <(awk -F ',' '{printf "%s %s\n", $4, $5}' "$SRC" \
    | sed 's/^[[:space:]]*//g' \
    | sed 's/ \+/ /g') \
    | cut -d ' ' -f1 \
    | sort \
    | uniq -c \
    | head -n 1 \
    | sed 's/^[[:space:]]*//g' \
    | cut -d ' ' -f2)"

while read line; do
    constellation="$(echo "$line" \
        | cut -d ',' -f4 \
        | sed 's/^[[:space:]]*//g')"
    if [[ "$constellation" = "$CONSTELLATION" ]]; then
        echo "$line"
    fi
done < <(cat "$SRC") \
    | sort -t ',' -k 7 -n \
    | grep -v -E '^*,--$' \
    | head -n 1 \
    | cut -d ',' -f1
