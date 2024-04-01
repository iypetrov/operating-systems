#!/bin/bash

sum_digs () {
    echo "$1" \
        | tr -d '-' \
        | sed 's/ */ /g' \
        | sed 's/^ \+//g' \
        | tr ' ' '\n' \
        | awk '{sum+=$1} END{print sum}' 
}

TRG="$1"
INP="$2"

if [[ "$TRG" = "a" ]]; then
    nums="$(cat "$INP" | grep -E '^-?[0-9]+$')"

    max=$(echo "$nums" \
        | tr -d '-' \
        | sort -nr \
        | head -n 1)
    
    echo "$nums" | grep -E "^-?${max}$"
else 
    nums="$(cat "$INP" | grep -E '^-?[0-9]+$')"
    while read num; do
        echo "$num $(sum_digs "$num")"
    done < <(echo "$nums") | sort -t ' ' -k2,2nr -k1,1n | head -n 1 | cut -d ' ' -f1 
fi
