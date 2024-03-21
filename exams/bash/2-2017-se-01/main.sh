#!/bin/bash

all_broken_symb_link() {
    while read line; do
        if ! [[ -e "$line" ]]; then
            echo "$line"
        fi
    done < <(find "$1" -type l | xargs readlink -f)
}

min_number_hard_links() {
    while read line; do
        hl=$(echo "$line" | cut -d ' ' -f2)
        if [[ $hl -ge $1 ]]; then
            echo "$(echo "$line" | cut -d ' ' -f1)"
        fi
    done < <(find "$2" -type f | xargs stat -c "%n %h")
}

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "provide 1/2 arg(s)"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 args should be a dir"
    exit 99
fi

if [[ $# -eq 1 ]]; then
    all_broken_symb_link "$1"
else
    if ! [[ $2 =~ ^[0-9]+$ ]]; then
        echo "2 args should be a number"
        exit 99
    fi
    min_number_hard_links $2 "$1"
fi
