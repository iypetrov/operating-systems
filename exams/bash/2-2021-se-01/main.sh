#!/bin/bash

# use du for solution 

get_megabytes_files() {
    ADR="$1"

    sum=0
    while read file; do
        bytes=$(realpath "$file")
        megabytes="$(echo "$bytes 1048576" | awk '{printf  "%.0f\n", $1/$2}')"
        rfile="$(realpath "$file")"
        echo "$megabytes $rfile"
        sum=$((sum + megabytes))
    done < <(cat "$ADR" \
        | tail -n +2 \
        | sed 's/^[ \t]+//g')

    echo "$sum"
}

USER="$(whoami)"

if [[ "$USER" != "oracle" && "$USER" != "grid" ]]; then
    echo "you don't have perm to run this"
    exit 99
fi

get_megabytes_files "/u01/app/$USER/adrci"
